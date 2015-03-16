#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <float.h>
#include <math.h>


#define _WITH_CPINFO
#include <jvm/jvm.h>

#include "jconfig.h"


#define R8(b)		{ jcheck(__read(j, (void*) (b), 1) == 1); }
#define R16(b)		{ jcheck(__read(j, (void*) (b), 2) == 2); *(uint16_t*) (b) = j_bswap16(*(uint16_t*) (b)); }
#define R32(b)		{ jcheck(__read(j, (void*) (b), 4) == 4); *(uint32_t*) (b) = j_bswap32(*(uint32_t*) (b)); }
#define R64(b)		{ jcheck(__read(j, (void*) (b), 8) == 8); *(uint64_t*) (b) = j_bswap64(*(uint64_t*) (b)); }
#define RXX(b, s)	{ jcheck(__read(j, (void*) (b), s) == s); }


static inline int __read(jassembly_t* j, void* buffer, size_t size) {
	if(unlikely(j->position + size > j->size))
		return 0;

	memcpy(buffer, (void*) ((uint32_t) j->buffer + j->position), size);
	j->position += size;

	return size;
}


int jclass_cp_to_class(cpvalue_t* v, cpclass_t* c) {
	jcheck(v && c);

	c->tag = v->tag;
	c->name_index = v->value;

	return 0;
}

int jclass_cp_to_field(cpvalue_t* v, cpfield_t* c) {
	jcheck(v && c);

	c->tag = v->tag;
	c->class_index = (v->value >> 16) & 0xFFFF;
	c->typename_index = v->value & 0xFFFF;

	return 0;
}


int jclass_cp_to_string(cpvalue_t* v, cpstring_t* c) {
	jcheck(v && c);

	c->tag = v->tag;
	c->string_index = v->value;

	return 0;
}


int jclass_cp_to_int(cpvalue_t* v, cpint_t* c) {
	jcheck(v && c);

	c->tag = v->tag;
	c->value = v->value;

	return 0;
}

int jclass_cp_to_long(cpvalue_t* v, cplong_t* c) {
	jcheck(v && c);

	c->tag = v->tag;
	c->value = v->value;

	return 0;
}

int jclass_cp_to_float(cpvalue_t* v, cpfloat_t* c) {
	jcheck(v && c);

	c->tag = v->tag;
	
	switch(v->value) {
		case 0x7F800000:
			c->value = INFINITY;
			break;
		case 0xFF800000:
			c->value = -INFINITY;
			break;
		case 0x7F800001 ... 0x7FFFFFFF:
		case 0xFF800001 ... 0xFFFFFFFF:
			c->value = NAN;
			break;
		default: {
			int s = ((v->value >> 31) == 0) ? 1 : -1;
			int e = ((v->value >> 23) & 0xFF);
			int m = (e == 0)
						? (v->value & 0x7FFFFF) << 1
						: (v->value & 0x7FFFFF) | 0x800000;

			c->value = s * m * 2e-150L;
		}
	}

	return 0;
}

int jclass_cp_to_double(cpvalue_t* v, cpdouble_t* c) {
	jcheck(v && c);

	c->tag = v->tag;
	
	switch(v->value) {
		case 0x7FF0000000000000L:
			c->value = INFINITY;
			break;
		case 0xFFF0000000000000L:
			c->value = -INFINITY;
			break;
		case 0x7FF0000000000001L ... 0x7FFFFFFFFFFFFFFFL:
		case 0xFFF0000000000001L ... 0xFFFFFFFFFFFFFFFFL:
			c->value = NAN;
			break;
		default: {
			int s = ((v->value >> 63) == 0) ? 1 : -1;
			int e = ((v->value >> 52) & 0x7FFL);
			int m = (e == 0)
						? (v->value & 0xFFFFFFFFFFFFFL) << 1
						: (v->value & 0xFFFFFFFFFFFFFL) | 0x10000000000000L;

			c->value = s * m * 2e-1075L;
		}
	}

	return 0;
}


int jclass_cp_to_typename(cpvalue_t* v, cptypename_t* c) {
	jcheck(v && c);

	c->tag = v->tag;
	c->name_index = (v->value >> 16) & 0xFFFF;
	c->desc_index = v->value & 0xFFFF;

	return 0;
}

int jclass_cp_to_utf8(cpvalue_t* v, cputf8_t* c) {
	jcheck(v && c);

	c->tag = v->tag;
	c->length = v->value;
	c->value = v->data;

	return 0;
}


int jclass_default_attribute(char* name) {
	jcheck(name);

	if(strcmp(name, "ConstantValue") == 0)
		return JCLASS_ATTR_CONSTVALUE;
	if(strcmp(name, "SourceFile") == 0)
		return JCLASS_ATTR_SOURCEFILE;
	if(strcmp(name, "Code") == 0)
		return JCLASS_ATTR_CODE;
	if(strcmp(name, "Exceptions") == 0)
		return JCLASS_ATTR_EXCEPTIONS;
	if(strcmp(name, "InnerClasses") == 0)
		return JCLASS_ATTR_INNERCLASS;
	if(strcmp(name, "Synthetic") == 0)
		return JCLASS_ATTR_SYNTHETIC;
	if(strcmp(name, "LineNumberTable") == 0)
		return JCLASS_ATTR_LINENUMBERS;
	if(strcmp(name, "LocalVariableTable") == 0)
		return JCLASS_ATTR_LOCALVARS;
	if(strcmp(name, "Deprecated") == 0)
		return JCLASS_ATTR_DEPRECATED;
	if(strcmp(name, "Signature") == 0)
		return JCLASS_ATTR_SIGNATURE;
	if(strcmp(name, "StackMapTable") == 0)
		return JCLASS_ATTR_STACKMAPTABLE;

#ifdef DEBUG
	j_printf("attribute %s not found\n", name);
#endif
	return JCLASS_ATTR_UNKNOWN;
}

int jclass_get_utf8_from_cp(jassembly_t* j, cputf8_t* utf, int idx) {
	cpvalue_t* v = (cpvalue_t*) list_at_index(j->header.jc_cpinfo, idx - 1);
	jcheck(v);

	jcheck(jclass_cp_to_utf8(v, utf) == 0);
	return 0;
}


int jclass_parse_attributes(jassembly_t* j, list_t* attributes, int attr_count) {

	if(attr_count == 0)
		return 0;

	int i, k;
	for(k = 0; k < attr_count; k++) {
		uint16_t idx = 0;
		uint32_t ln = 0;
		R16(&idx);
		R32(&ln);			

	
		cputf8_t utf;
		jclass_get_utf8_from_cp(j, &utf, idx);


		switch(jclass_default_attribute(utf.value)) {
			case JCLASS_ATTR_CONSTVALUE: {
					attr_cvalue_t* cc = (attr_cvalue_t*) jmalloc(sizeof(attr_cvalue_t));

					cc->name_index = idx;
					cc->length = ln;

					R16(&cc->cvalue_index);

					jcheck(list_add(attributes, (listval_t) cc) == 0);
				}
				break;
			case JCLASS_ATTR_SOURCEFILE: {
					attr_sourcefile_t* cc = (attr_sourcefile_t*) jmalloc(sizeof(attr_sourcefile_t));

					cc->name_index = idx;
					cc->length = ln;

					R16(&cc->source_index);

					jcheck(list_add(attributes, (listval_t) cc) == 0);
				}
				break;
			case JCLASS_ATTR_CODE: {
					attr_code_t* cc = (attr_code_t*) jmalloc(sizeof(attr_code_t));

					cc->name_index = idx;
					cc->length = ln;

					R16(&cc->max_stack);
					R16(&cc->max_locals);
					R32(&cc->code_length);
						
					cc->code = (uint8_t*) jmalloc(cc->code_length);
					RXX(cc->code, cc->code_length);

					R16(&cc->exception_table_length);

					cc->exceptions = (exception_table_t*) jmalloc(sizeof(exception_table_t) * cc->exception_table_length);
					for(i = 0; i < cc->exception_table_length; i++) {
						R16(&cc->exceptions[i].start_pc);
						R16(&cc->exceptions[i].end_pc);
						R16(&cc->exceptions[i].handler_pc);
						R16(&cc->exceptions[i].catch_type);
					}

					R16(&cc->attr_count);						
					list_init(cc->attributes);

					jcheck(jclass_parse_attributes(j, cc->attributes, cc->attr_count) == 0);
					jcheck(list_add(attributes, (listval_t) cc) == 0);
				}
				break;
			case JCLASS_ATTR_EXCEPTIONS: {
					attr_exception_t* cc = (attr_exception_t*) jmalloc(sizeof(attr_exception_t));
					cc->name_index = idx;
					cc->length = ln;

					R16(&cc->exceptions_count);

					cc->exceptions = (uint16_t*) jmalloc(sizeof(uint16_t) * cc->exceptions_count);
					for(i = 0; i < cc->exceptions_count; i++)
						R16(&cc->exceptions[i]);

					jcheck(list_add(attributes, (listval_t) cc) == 0);
				}
				break;
			case JCLASS_ATTR_INNERCLASS: {
					attr_iclass_t* cc = (attr_iclass_t*) jmalloc(sizeof(attr_iclass_t));
					cc->name_index = idx;
					cc->length = ln;

					R16(&cc->classes_count);
					
					cc->classes = (innerclass_t*) jmalloc(sizeof(innerclass_t) * cc->classes_count);
					for(i = 0; i < cc->classes_count; i++) {
						R16(&cc->classes[i].info_index);
						R16(&cc->classes[i].super_index);
						R16(&cc->classes[i].name_index);
						R16(&cc->classes[i].access);
					}

					jcheck(list_add(attributes, (listval_t) cc) == 0);
				}
				break;
			case JCLASS_ATTR_DEPRECATED:
			case JCLASS_ATTR_SYNTHETIC: {
					attrinfo_t* cc = (attrinfo_t*) jmalloc(sizeof(attrinfo_t));
					cc->name_index = idx;
					cc->length = ln;

					jcheck(list_add(attributes, (listval_t) cc) == 0);
				}
				break;
			case JCLASS_ATTR_LINENUMBERS: {
					attr_linenumbers_t* cc = (attr_linenumbers_t*) jmalloc(sizeof(attr_linenumbers_t));
					cc->name_index = idx;
					cc->length = ln;

					R16(&cc->lines_count);	
					
					cc->table = (linenumber_table_t*) jmalloc(sizeof(linenumber_table_t) * cc->lines_count);
					for(i = 0; i < cc->lines_count; i++) {
						R16(&cc->table[i].pc);
						R16(&cc->table[i].line);
					}

					jcheck(list_add(attributes, (listval_t) cc) == 0);
				}
				break;
			case JCLASS_ATTR_LOCALVARS: {
					attr_localvars_t* cc = (attr_localvars_t*) jmalloc(sizeof(attr_localvars_t));
					cc->name_index = idx;
					cc->length = ln;
			
					R16(&cc->vars_count);

					cc->table = (localvars_table_t*) jmalloc(sizeof(localvars_table_t) * cc->vars_count);
					for(i = 0; i < cc->vars_count; i++) {
						R16(&cc->table[i].pc);
						R16(&cc->table[i].length);
						R16(&cc->table[i].name_index);
						R16(&cc->table[i].desc_index);
						R16(&cc->table[i].index);
					}

					jcheck(list_add(attributes, (listval_t) cc) == 0);
				}
				break;
			case JCLASS_ATTR_SIGNATURE: {
					attr_sign_t* cc = (attr_sign_t*) jmalloc(sizeof(attr_sign_t));
					cc->name_index = idx;
					cc->length = ln;

					R16(&cc->sign_index);

					jcheck(list_add(attributes, (listval_t) cc) == 0);
				}
				break;
			case JCLASS_ATTR_STACKMAPTABLE: {
					attr_stackmap_t* cc = (attr_stackmap_t*) jmalloc(sizeof(attr_stackmap_t));
					cc->name_index = idx;
					cc->length = ln;
		
					R16(&cc->frames_count);

					cc->frames = (void*) jmalloc(cc->length - 2);
					RXX(cc->frames, (cc->length - 2));

					jcheck(list_add(attributes, (listval_t) cc) == 0);
				}
				break;
			case JCLASS_ATTR_UNKNOWN:
			default: {
				attrinfo_t* cc = (attrinfo_t*) jmalloc(sizeof(attrinfo_t) + ln);
				cc->name_index = idx;
				cc->length = ln;

				RXX(((void*) ((uint32_t) cc + 6)), ln);

				jcheck(list_add(attributes, (listval_t) cc) == 0);
			}
		}
	}

	return 0;
}


int jclass_parse_desc(const char* desc, uint8_t* nargs, uint8_t* rettype, char* signature) {
	jcheck(desc);

	if(desc[0] != '(')
		goto noparams;
	desc++;

	
	jcheck(nargs && signature);

	while(*desc && *desc != ')') {
		switch(*desc) {
			case '[':
				while(*desc == '[')
					desc++;
				desc--;
			case 'L':
				desc++;
				switch(*desc) {
					case 'B':
					case 'C':
					case 'D':
					case 'F':
					case 'I':
					case 'J':
					case 'S':
					case 'Z':
					case 'V':
						break;
					default:
						desc = strchr(desc, ';');
						jcheck(desc);
						break;
				}
				*signature++ = 'L';
				break;
			default:
				*signature++ = *desc;
		}

		
		
		*nargs += 1;
		desc++;
	}


	if(*desc != ')')
		return -1;
	desc++;

noparams:

	jcheck(rettype);

	switch(*desc) {
		case 'B':
			*rettype = T_BYTE;
			break;
		case 'C':
			*rettype = T_CHAR;
			break;
		case 'D':
			*rettype = T_DOUBLE;
			break;
		case 'F':
			*rettype = T_FLOAT;
			break;
		case 'I':
			*rettype = T_INT;
			break;
		case 'J':
			*rettype = T_LONG;
			break;
		case 'L':
		case '[':
			*rettype = T_REFERENCE;
			break;
		case 'S':
			*rettype = T_SHORT;
			break;
		case 'Z':
			*rettype = T_BOOLEAN;
			break;
		case 'V':
			*rettype = T_VOID;
			break;
		default:
			jprintf("warning: return type %c is undefined\n", *desc);
			*rettype = T_VOID;
	}

	return 0;
}


int jclass_resolve_method(jassembly_t* j, methodinfo_t* method) {
	jcheck(j && method);

	cputf8_t utf_n, utf_d;
	jcheck(jclass_get_utf8_from_cp(j, &utf_n, method->name_index) == 0);
	jcheck(jclass_get_utf8_from_cp(j, &utf_d, method->desc_index) == 0);

	
	if(method->access & ACC_NATIVE) {
		jnative_t* native = (jnative_t*) jnative_find_method(j->name, utf_n.value);
		if(unlikely(!native)) {
#ifdef DEBUG
			jprintf("native method %s.%s not found\n", j->name, utf_n.value);
#endif			
			j_throw(NULL, JEXCEPTION_UNSATISFIED_LINK);
		}

		method->nargs = strlen(native->signature);
		method->rettype = native->rettype;
		strcpy(method->signature, native->signature);
	} else
		jcheck(jclass_parse_desc(utf_d.value, &method->nargs, &method->rettype, method->signature) == 0);
	


	method->classname = (char*) strdup(j->name);
	method->name = (char*) strdup(utf_n.value);
	method->code = (attr_code_t*) jcode_find_attribute(j, method->attributes, "Code");

	return 0;
}

int jclass_resolve_field(jassembly_t* j, fieldinfo_t* field) {
	jcheck(j && field);

	cputf8_t utf_n, utf_d;
	jcheck(jclass_get_utf8_from_cp(j, &utf_n, field->name_index) == 0);
	jcheck(jclass_get_utf8_from_cp(j, &utf_d, field->desc_index) == 0);
	
	jcheck(jclass_parse_desc(utf_d.value, NULL, &field->type, NULL) == 0);

	field->classname = (char*) strdup(j->name);
	field->name = (char*) strdup(utf_n.value);
	field->value.u64 = 0UL;
	
	attr_cvalue_t* cv = (attr_cvalue_t*) jcode_find_attribute(j, field->attributes, "ConstantValue");
	if(unlikely(!cv))
		return 0;


	cpvalue_t* cp = (cpvalue_t*) list_at_index(j->header.jc_cpinfo, cv->cvalue_index - 1);
	jcheck(cp);

	if(cp->tag == JCLASS_TAG_STRING)
		cp = (cpvalue_t*) list_at_index(j->header.jc_cpinfo, cp->value - 1);

	jcheck(cp);


	if(cp->tag == JCLASS_TAG_UTF8STRING)
		field->value.ptr = cp->data;
	else
		field->value.i64 = cp->value;

	return 0;
}


int jclass_resolve_dep(jassembly_t* j, cpvalue_t* cp, int index) {
	jcheck(j && cp);

	cpclass_t cl;
	jcheck(jclass_cp_to_class(cp, &cl) == 0);


	cputf8_t utf;
	jcheck(jclass_get_utf8_from_cp(j, &utf, cl.name_index) == 0);

	
	if(strcmp(j->name, utf.value) == 0)
		return 0;

	if(unlikely(utf.value[0] == '['))
		return 0;		/* Array references */

	

	jassembly_t* dep = NULL;

	if(unlikely(jassembly_load(&dep, utf.value) != 0)) {
#ifdef DEBUG
		jprintf("could not resolve \"%s\" from %s\n", utf.value, j->name);
#endif
		return 0;
	}

	jcheck(list_add(j->deps, (listval_t) dep) == 0);

	dep->index = index;

	return 0;
}

int jclass_resolve_deps(jassembly_t* j) {
	jcheck(j);

	int index = 1;
	list_foreach(value, j->header.jc_cpinfo) {
		cpvalue_t* cc = (cpvalue_t*) value;
		
		if(cc->tag == JCLASS_TAG_CLASS)
			jcheck(jclass_resolve_dep(j, cc, index) == 0);

		index++;
	}

	return 0;
}

int jclass_resolve_super(jassembly_t* j) {
	if(j->header.jc_super == 0)
		return 0;

	cpvalue_t* cp = (cpvalue_t*) list_at_index(j->header.jc_cpinfo, j->header.jc_super - 1);
	jcheck(cp);

	cpclass_t cl;
	jcheck(jclass_cp_to_class(cp, &cl) == 0);

	cputf8_t utf;
	jcheck(jclass_get_utf8_from_cp(j, &utf, cl.name_index) == 0);

	
	j->super = (jassembly_t*) jassembly_find(j, utf.value);
	jcheck(j->super);
	return 0;
}


int jclass_resolve_name(jassembly_t* j) {
	jcheck(j);

	cpvalue_t* cp = (cpvalue_t*) list_at_index(j->header.jc_cpinfo, j->header.jc_this - 1);
	jcheck(cp);

	cpclass_t cl;
	jcheck(jclass_cp_to_class(cp, &cl) == 0);

	cputf8_t utf;
	jcheck(jclass_get_utf8_from_cp(j, &utf, cl.name_index) == 0);
	 

	j->name = (char*) strdup(utf.value);
	return 0;
}


int jclass_parse_assembly(jassembly_t* j) {
	jcheck(j);


	int i = 0;

	R32(&j->header.jc_magic);
	jcheck(j->header.jc_magic == JCLASS_MAGIC);

	R16(&j->header.jc_minor);
	R16(&j->header.jc_major);
	R16(&j->header.jc_cp_count);


	list_init(j->header.jc_cpinfo);
	list_init(j->header.jc_fields);
	list_init(j->header.jc_methods);
	list_init(j->header.jc_attributes);

	if(j->deps == NULL)
		{ list_init(j->deps); }


	for(i = 0; i < j->header.jc_cp_count - 1; i++) {
		uint8_t tag = 0;
		uint64_t value = 0;
		R8(&tag);

		switch(cpinfo[tag].cp_len) {
			case 0:
				break;
			case 1:
				R8(&value);
				break;
			case 2:
				R16(&value);
				break;
			case 4:
				R32(&value);
				break;
			case 8:
				R64(&value);
				break;
			default:
				jerror("Invalid TAG");
		}

		
		cpvalue_t* cp = (cpvalue_t*) jmalloc(sizeof(cpvalue_t));
		cp->tag = tag;
		cp->value = value;
		cp->data = NULL;

		jcheck(list_add(j->header.jc_cpinfo, (listval_t) cp) == 0);
		

		switch(tag) {
			case JCLASS_TAG_LONG:
			case JCLASS_TAG_DOUBLE:
				jcheck(list_add(j->header.jc_cpinfo, (listval_t) jmalloc(sizeof(cpvalue_t))) == 0);
				i++;
				break;
			case JCLASS_TAG_UTF8STRING:
				cp->data = (char*) jmalloc(cp->value + 1);
				memset(cp->data, 0, cp->value + 1);
				RXX(cp->data, cp->value);
				break;
			default:
				continue;
		}
	}

	
	R16(&j->header.jc_access);
	R16(&j->header.jc_this);
	R16(&j->header.jc_super);
	R16(&j->header.jc_interfaces_count);

	jcheck(jclass_resolve_name(j) == 0);

	j->index = j->header.jc_this;
	j->header.jc_interfaces = (uint16_t*) jmalloc(j->header.jc_interfaces_count * sizeof(uint16_t));

	
	for(i = 0; i < j->header.jc_interfaces_count; i++)
		R16(&j->header.jc_interfaces[i]);
	

	R16(&j->header.jc_fields_count);

	for(i = 0; i < j->header.jc_fields_count; i++) {
		fieldinfo_t* field = (fieldinfo_t*) jmalloc(sizeof(fieldinfo_t));

		R16(&field->access);
		R16(&field->name_index);
		R16(&field->desc_index);
		R16(&field->attr_count);

		list_init(field->attributes);

		jcheck(jclass_parse_attributes(j, field->attributes, field->attr_count) == 0);
		jcheck(jclass_resolve_field(j, field) == 0);
		jcheck(list_add(j->header.jc_fields, (listval_t) field) == 0);
	}

	R16(&j->header.jc_methods_count);

	for(i = 0; i < j->header.jc_methods_count; i++) {
		methodinfo_t* method = (methodinfo_t*) jmalloc(sizeof(methodinfo_t));

		R16(&method->access);
		R16(&method->name_index);
		R16(&method->desc_index);
		R16(&method->attr_count);


		list_init(method->attributes);
	
		jcheck(jclass_parse_attributes(j, method->attributes, method->attr_count) == 0);
		jcheck(jclass_resolve_method(j, method) == 0);
		jcheck(list_add(j->header.jc_methods, (listval_t) method) == 0);
	}

	R16(&j->header.jc_attributes_count);

	jcheck(jclass_parse_attributes(j, j->header.jc_attributes, j->header.jc_attributes_count) == 0);
	
	return 0;
}

int jclass_resolve_assembly(jassembly_t* j) {
	jcheck(jclass_resolve_deps(j) == 0);
	jcheck(jclass_resolve_super(j) == 0);

	return 0;
}
