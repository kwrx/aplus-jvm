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

static jcontext_t* __current_ctx = NULL;
list_t* jnative_handlers;


int jnative_register_function(const char* classname, const char* name, const char* signature, int16_t rettype, void* handler) {

	jcheck(name && handler);

	if(!jnative_handlers) {
		list_init(jnative_handlers);
	}

	if(strcmp(signature, "V"))
		signature = (char*) strdup("");

	jnative_t* cc = (jnative_t*) jmalloc(sizeof(jnative_t));
	cc->classname = (char*) strdup(classname);
	cc->name = (char*) strdup(name);
	cc->signature = (char*) strdup(signature);
	cc->rettype = rettype;
	cc->handler = handler; 

	jcheck(list_add(jnative_handlers, (listval_t) cc) == 0);
	return 0;
}

int jnative_unregister_function(const char* classname, const char* name) {
	jcheck(name);

	if(!jnative_handlers) {
		list_init(jnative_handlers);
	}

	if(list_empty(jnative_handlers))
		return -1;

	
	jnative_t* to_remove = NULL;
	list_foreach(value, jnative_handlers) {
		jnative_t* cc = (jnative_t*) value;

		if((strcmp(cc->name, name) == 0) && (strcmp(cc->classname, classname) == 0)) {
			to_remove = cc;
			break;
		}
	}

	if(to_remove)
		list_remove(jnative_handlers, (listval_t) to_remove);

	return 0;
}

jnative_t* jnative_find_method(const char* classname, const char* name) {
	jcheck(name);

	if(!jnative_handlers) {
		list_init(jnative_handlers);
	}

	if(list_empty(jnative_handlers))
		return NULL;

	list_foreach(value, jnative_handlers) {
		jnative_t* cc = (jnative_t*) value;

		if((strcmp(cc->name, name) == 0) && (strcmp(cc->classname, classname) == 0))
			return cc;
	}

	return NULL;
}


void jnative_set_context(jcontext_t* j) {
	__current_ctx = j;
}

jcontext_t* jnative_get_context() {
	jcheck(__current_ctx);
	return __current_ctx;
}
