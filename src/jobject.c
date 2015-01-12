#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <float.h>
#include <math.h>


#include <jvm/jvm.h>
#include "jconfig.h"


static void* __clone(void* ptr, size_t size) {
	void* pp = (void*) jmalloc(size);
	jcheck(pp);

	memcpy(pp, ptr, size);
	return pp;
}


uint64_t jobject_id() {
	static uint64_t object_id = 0;
	return object_id++;
}

jobject_t* jobject_new(jassembly_t* j, int16_t idx) {

	cpvalue_t* v = (cpvalue_t*) list_at_index(j->header.jc_cpinfo, idx - 1);
	jcheck(v);

	cpclass_t cclass;
	jclass_cp_to_class(v, &cclass);

	cputf8_t utf;
	jclass_get_utf8_from_cp(j, &utf, cclass.name_index);


	jassembly_t* j2 = (jassembly_t*) jassembly_find(j, utf.value);
	jcheck(j2);

	jobject_t* obj = (jobject_t*) jmalloc(sizeof(jobject_t));
	obj->refcount = 0;
	obj->lock = 0;
	obj->fullname = obj->name = (char*) strdup(utf.value);
	obj->assembly = j2;
	obj->id = jobject_id();
	
	list_init(obj->fields);

	list_foreach(value, j2->header.jc_fields)
		jcheck(list_add(obj->fields, (listval_t) __clone((void*) value, sizeof(fieldinfo_t))) == 0);


	return obj;
}

jobject_t* jobject_new_from_assembly(jassembly_t* j, jassembly_t* cc) {
	jobject_t* obj = (jobject_t*) jmalloc(sizeof(jobject_t));
	obj->refcount = 0;
	obj->lock = 0;
	obj->fullname = obj->name = (char*) strdup(cc->name);
	obj->assembly = cc;
	obj->id = jobject_id();
	
	list_init(obj->fields);

	list_foreach(value, cc->header.jc_fields)
		jcheck(list_add(obj->fields, (listval_t) __clone((void*) value, sizeof(fieldinfo_t))) == 0);


	return obj;
}

void jobject_finalize(jobject_t* obj) {
	jcheck(obj);

#if !defined(__GLIBC__)
	list_foreach(value, obj->fields)
		jfree((void*) value);

	list_destroy(obj->fields);
	jfree(obj);
#endif
}

jobject_t* jobject_clone(jobject_t* obj) {
	jcheck(obj);

	jobject_t* cc = __clone(obj, sizeof(jobject_t));
	cc->id = jobject_id();

	return cc;
}


fieldinfo_t* jobject_get_field(jobject_t* obj, const char* name, uint8_t type) {
	jcheck(obj && name);
	jcheck(obj->fields);

	list_foreach(value, obj->fields) {
		if(
			(strcmp(((fieldinfo_t*) value)->name, name) == 0) && 
			(((fieldinfo_t*) value)->type == type)
		) return (fieldinfo_t*) value;
	}

	return NULL;
}

jvalue_t jobject_get_fieldvalue(jobject_t* obj, const char* name, uint8_t type) {
	fieldinfo_t* v = NULL;	
	if(likely(v = jobject_get_field(obj, name, type)))
		return v->value;

	return (jvalue_t) 0LL;
}

void jobject_set_fieldvalue(jobject_t* obj, const char* name, uint8_t type, jvalue_t value) {
	fieldinfo_t* v = NULL;	
	if(likely(v = jobject_get_field(obj, name, type)))
		v->value = value;
}

