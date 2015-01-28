#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>


#define _GNU_SOURCE
#include <float.h>
#include <math.h>


#include <jvm/jvm.h>
#include "jconfig.h"

#define _WITH_OPCODES
#include "opcodes/opcode.h"


int jcode_context_run(jcontext_t* j) {
	jcheck(j);

	int cr = setjmp(j->retbuf);
	if(unlikely(cr))
		return cr;

	j->regs.pc = 0;
	j->regs.pb = 0;

	while(1) {
		register int opcode = j->code[j->regs.pc];

#if defined(DEBUG)
		if(unlikely(j_opcodes[opcode].handler == NULL)) {
			j_printf("Wrong opcode: %X (%d) at %d\n", opcode, opcode, j->regs.pc);
			j_throw(j, JEXCEPTION_INVALID_OPCODE);
		}
#endif

#if defined(DEBUG) && defined(VERBOSE)
		j_printf("[%d] (%2X) %s\n", j->regs.pc, opcode, j_opcodes[opcode].name);
#endif
	
		j->regs.pb = j->regs.pc;
		j->regs.pc += 1;
		j_opcodes[opcode].handler (j);
	}

	return 0;
}

methodinfo_t* jcode_find_method(jassembly_t* j, const char* name) {
	if(j->header.jc_methods_count) {
		list_foreach(value, j->header.jc_methods) {
			methodinfo_t* v = (methodinfo_t*) value;

			cputf8_t utf;
			jcheck(jclass_get_utf8_from_cp(j, &utf, v->name_index) == 0);
			
			if(strcmp(utf.value, name) == 0)
				return v;
		}
	}

	if(j->super)
		return jcode_find_method(j->super, name);

	return NULL;
}

methodinfo_t* jcode_find_method_and_desc(jassembly_t* j, const char* name, const char* desc) {
	if(j->header.jc_methods_count) {
		list_foreach(value, j->header.jc_methods) {
			methodinfo_t* v = (methodinfo_t*) value;

			cputf8_t utf, utf_d;
			jcheck(jclass_get_utf8_from_cp(j, &utf, v->name_index) == 0);
			jcheck(jclass_get_utf8_from_cp(j, &utf_d, v->desc_index) == 0);
			
			if((strcmp(utf.value, name) == 0) && (strcmp(utf_d.value, desc) == 0))
				return v;
		}
	}

	if(j->super)
		return jcode_find_method_and_desc(j->super, name, desc);

	return NULL;
}

methodinfo_t* jcode_find_method_and_desc_and_class(jassembly_t* j, const char* name, const char* desc, const char* classname) {

	j = (jassembly_t*) jassembly_find(j, classname);
	jcheck(j);

	
	list_foreach(value, j->header.jc_methods) {
		methodinfo_t* v = (methodinfo_t*) value;

		cputf8_t utf, utf_d;
		jcheck(jclass_get_utf8_from_cp(j, &utf, v->name_index) == 0);
		jcheck(jclass_get_utf8_from_cp(j, &utf_d, v->desc_index) == 0);
			
		if((strcmp(utf.value, name) == 0) && (strcmp(utf_d.value, desc) == 0))
				return v;
	}

	if(j->super)
		return jcode_find_method_and_desc(j->super, name, desc);

	return NULL;
}


methodinfo_t* jcode_find_methodref(jassembly_t* j, int16_t idx) {
	jcheck(j);

	cpvalue_t* v = (cpvalue_t*) list_at_index(j->header.jc_cpinfo, idx - 1);
	jcheck(v);

	cpmethod_t mref;
	jclass_cp_to_method(v, &mref);

	v = (cpvalue_t*) list_at_index(j->header.jc_cpinfo, mref.typename_index - 1);
	jcheck(v);

	cptypename_t ctype;
	jclass_cp_to_typename(v, &ctype);


	v = (cpvalue_t*) list_at_index(j->header.jc_cpinfo, mref.class_index - 1);
	jcheck(v);

	cpclass_t cclass;
	jclass_cp_to_class(v, &cclass);

	cputf8_t utf_n, utf_d, utf_c;
	jcheck(jclass_get_utf8_from_cp(j, &utf_n, ctype.name_index) == 0);
	jcheck(jclass_get_utf8_from_cp(j, &utf_d, ctype.desc_index) == 0);
	jcheck(jclass_get_utf8_from_cp(j, &utf_c, cclass.name_index) == 0);

	return jcode_find_method_and_desc_and_class(j, utf_n.value, utf_d.value, utf_c.value);
}

fieldinfo_t* jcode_find_fieldref(jassembly_t* j, list_t* fields, int16_t idx) {
	jcheck(j);

	cpvalue_t* v = (cpvalue_t*) list_at_index(j->header.jc_cpinfo, idx - 1);
	jcheck(v);

	cpfield_t mref;
	jclass_cp_to_method(v, &mref);


	v = (cpvalue_t*) list_at_index(j->header.jc_cpinfo, mref.typename_index - 1);
	jcheck(v);

	cptypename_t ctype;
	jclass_cp_to_typename(v, &ctype);


	v = (cpvalue_t*) list_at_index(j->header.jc_cpinfo, mref.class_index - 1);
	jcheck(v);

	cpclass_t cclass;
	jclass_cp_to_class(v, &cclass);

	cputf8_t utf_n, utf_d, utf_c;
	jcheck(jclass_get_utf8_from_cp(j, &utf_n, ctype.name_index) == 0);
	jcheck(jclass_get_utf8_from_cp(j, &utf_d, ctype.desc_index) == 0);
	jcheck(jclass_get_utf8_from_cp(j, &utf_c, cclass.name_index) == 0);

	char* desc = utf_d.value;
	char* name = utf_n.value;
	char* classname = utf_c.value;

	if(!fields) {
		j = (jassembly_t*) jassembly_find(j, classname);
		jcheck(j);

		fields = j->header.jc_fields;
	}

	jcheck(fields);
	
	list_foreach(value, fields) {
		fieldinfo_t* v = (fieldinfo_t*) value;

		cputf8_t utf, utf_d;
		jcheck(jclass_get_utf8_from_cp(j, &utf, v->name_index) == 0);
		jcheck(jclass_get_utf8_from_cp(j, &utf_d, v->desc_index) == 0);
			
		if((strcmp(utf.value, name) == 0) && (strcmp(utf_d.value, desc) == 0))
				return v;
	}

	return NULL;
}

attrinfo_t* jcode_find_attribute(jassembly_t* j, list_t* attributes, const char* name) {
	list_foreach(value, attributes) {
		attrinfo_t* v = (attrinfo_t*) value;


		cputf8_t utf1;
		jclass_get_utf8_from_cp(j, &utf1, v->name_index);
		
		if(strcmp(name, utf1.value) == 0)
			return v;
	}

	return NULL;
}


jvalue_t jcode_method_invoke(jassembly_t* jx, methodinfo_t* method, jvalue_t* params, int params_count) {
	jcheck(jx && method);

	if(params_count > 0)
		jcheck(params);


	if(method->access & ACC_NATIVE) {
		jnative_t* native = (jnative_t*) jnative_find_method(method->classname, method->name);
		if(unlikely(!native)) {
#ifdef DEBUG
			j_printf("native method %s.%s not found\n", method->classname, method->name);
#endif		
			j_throw(NULL, JEXCEPTION_UNSATISFIED_LINK);
		}

		return native_call(method, native, params, params_count);
	}



	if(unlikely(!method->code))
		j_throw(NULL, JEXCEPTION_ABSTRACT_METHOD);


	jcontext_t* j = (jcontext_t*) jmalloc(sizeof(jcontext_t));
	j->current_assembly = jx;

	j->locals = (jvalue_t*) jmalloc(sizeof(jvalue_t) * method->code->max_locals);
	j->locals_count = method->code->max_locals;

	j->stack = (jvalue_t*) jmalloc(sizeof(jvalue_t) * method->code->max_stack);
	j->stack_size = method->code->max_stack;
	j->stack_top = 0;

	j->code = method->code->code;
	j->method = method;

	int i, p;
	for(i = 0, p = 0; i < params_count; i++, p++) {

		j->locals[p] = params[i];

		switch(method->signature[i]) {
			case 'J':
			case 'D':
				p++;
				break;
		}	
	}		

#ifdef DEBUG
	j_printf("%s.%s (args: %d; stack: %d; locals: %d)\n", method->classname, method->name, method->nargs, method->code->max_stack, method->code->max_locals);
#endif


	jcode_context_run(j);
	jvalue_t retval = j->regs.r0;

	
#if !defined(__GLIBC__)
	jfree(j->locals);
	jfree(j->stack);
	jfree(j);
#endif

	return retval;
}


jvalue_t jcode_function_call(jassembly_t* j, const char* name, jvalue_t* params, int params_count) {
	jcheck(j && name);

	if(params_count > 0)
		jcheck(params);

	
	methodinfo_t* method = jcode_find_method(j, name);
	jcheck(method);

	return jcode_method_invoke(j, method, params, params_count);
}


