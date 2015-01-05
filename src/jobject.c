#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <float.h>
#include <math.h>

#include <assert.h>


#include <jvm/jvm.h>
#include "jconfig.h"


void* jobject_clone(void* ptr, size_t size) {
	void* pp = (void*) jmalloc(size);
	assert(pp);

	memcpy(pp, ptr, size);
	return pp;
}

jobject_t* jobject_new(jassembly_t* j, int16_t idx) {
	static uint64_t object_id = 0;

	cpvalue_t* v = (cpvalue_t*) list_at_index(j->header.jc_cpinfo, idx - 1);
	assert(v);

	cpclass_t cclass;
	assert(jclass_cp_to_class(v, &cclass) == 0);

	cputf8_t utf;
	assert(jclass_get_utf8_from_cp(j, &utf, cclass.name_index) == 0);

	jassembly_t* j2 = (jassembly_t*) jassembly_find(j, utf.value);
	assert(j2);

	jobject_t* obj = (jobject_t*) jmalloc(sizeof(jobject_t));
	obj->refcount = 0;
	obj->lock = 0;
	obj->fullname = obj->name = (char*) strdup(utf.value);
	obj->assembly = j2;
	obj->id = object_id++;
	
	list_init(obj->fields);

	list_foreach(value, j->header.jc_fields)
		assert(list_add(obj->fields, (listval_t) jobject_clone((void*) value, sizeof(fieldinfo_t))) == 0);


	return obj;
}

int jobject_destroy(jobject_t* obj) {
	assert(obj);

#if !defined(__GLIBC__)
	list_foreach(value, obj->fields)
		jfree((void*) value);

	list_destroy(obj->fields);
	jfree(obj);
#endif

	return 0;
}

