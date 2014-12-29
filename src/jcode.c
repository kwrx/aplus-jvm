#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>


#define _GNU_SOURCE
#include <float.h>
#include <math.h>

#include <assert.h>

#include <jvm/jvm.h>

#define _WITH_OPCODES
#include "opcodes/opcode.h"



int j_throw(jcontext_t* j, char* exception) {
	printf("Exception! %s\n", exception);
	exit(1);
}

int jcode_context_run(jcontext_t* j) {
	assert(j);

	if(setjmp(j->retbuf))
		return 0;

	j->regs.pc = 0;
	j->regs.pb = 0;

	while(1) {
		if(j_opcodes[j->code[j->regs.pc]].handler == NULL)
			j_throw(j, JEXCEPTION_INVALID_OPCODE);

#ifdef DEBUG
		j_printf("[%d] %s\n", j->regs.pc, j_opcodes[j->code[j->regs.pc]].name);
#endif
		
		j->regs.pb = j->regs.pc;
		j->regs.pc += 1;
		j_opcodes[j->code[j->regs.pc - 1]].handler (j);
	}

	return 0;
}

methodinfo_t* jcode_find_method(jassembly_t* j, const char* name) {
	if(j->header.jc_methods_count) {
		list_foreach(value, j->header.jc_methods) {
			methodinfo_t* v = (methodinfo_t*) value;

			cputf8_t utf;
			jclass_get_utf8_from_cp(j, &utf, v->name_index);
			
			if(strcmp(utf.value, name) == 0)
				return v;
		}
	}

	return NULL;
}

methodinfo_t* jcode_find_methodref(jassembly_t* j, int16_t idx) {
	assert(j);

	cpvalue_t* v = (cpvalue_t*) list_at_index(j->header.jc_cpinfo, idx - 1);
	assert(v);

	cpmethod_t mref;
	assert(jclass_cp_to_method(v, &mref) == 0);




	v = (cpvalue_t*) list_at_index(j->header.jc_cpinfo, mref.typename_index - 1);
	assert(v);

	cptypename_t ctype;
	assert(jclass_cp_to_typename(v, &ctype) == 0);


	cputf8_t utf;
	assert(jclass_get_utf8_from_cp(j, &utf, ctype.name_index) == 0);

	return jcode_find_method(j, utf.value);
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


jvalue_t jcode_method_invoke(jassembly_t* j, methodinfo_t* method, jvalue_t* params, int params_count) {
	assert(j && method);

	if(params_count > 0)
		assert(params);


	jcontext_t* context = (jcontext_t*) jmalloc(sizeof(jcontext_t));
	context->current_assembly = j;

	context->locals = (jvalue_t*) jmalloc(sizeof(jvalue_t) * method->code->max_locals);
	context->locals_count = method->code->max_locals;

	context->stack = (jvalue_t*) jmalloc(sizeof(jvalue_t) * method->code->max_stack);
	context->stack_size = method->code->max_stack;
	context->stack_top = 0;

	context->code = method->code->code;

	int i, p;
	for(i = 0, p = 0; i < params_count; i++, p++) {

		context->locals[p] = params[i];

		switch(method->signature[i]) {
			case 'J':
			case 'D':
				p++;
				break;
		}	
	}		

#ifdef DEBUG
	j_printf("%s (args: %d; stack: %d; locals: %d)\n", method->name, method->nargs, method->code->max_stack, method->code->max_locals);
#endif


	jcode_context_run(context);
	jvalue_t retval = context->regs.r0;


#if !defined(__GLIBC__)
	jfree(context->locals);
	jfree(context->stack);
	jfree(context);
#endif

	return retval;
}


jvalue_t jcode_function_call(jassembly_t* j, const char* name, jvalue_t* params, int params_count) {
	assert(j && name);

	if(params_count > 0)
		assert(params);

	
	methodinfo_t* method = jcode_find_method(j, name);
	assert(method);

	return jcode_method_invoke(j, method, params, params_count);
}


