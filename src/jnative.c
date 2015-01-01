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


list_t* jnative_handlers;


int jnative_register_function(const char* name, const char* signature, int16_t rettype, jnative_handler_t handler) {

	assert(name && handler);

	if(!jnative_handlers) {
		list_init(jnative_handlers);
	}

	jnative_t* cc = (jnative_t*) jmalloc(sizeof(jnative_t));
	cc->name = (char*) strdup(name);
	cc->signature = (char*) strdup(signature);
	cc->handler = handler; 

	assert(list_add(jnative_handlers, (listval_t) cc) == 0);
	return 0;
}

int jnative_unregister_function(const char* name) {
	assert(name);

	if(!jnative_handlers) {
		list_init(jnative_handlers);
	}

	if(list_empty(jnative_handlers))
		return -1;

	
	jnative_t* to_remove = NULL;
	list_foreach(value, jnative_handlers) {
		jnative_t* cc = (jnative_t*) value;

		if(strcmp(cc->name, name) == 0) {
			to_remove = cc;
			break;
		}
	}

	if(to_remove)
		list_remove(jnative_handlers, (listval_t) to_remove);

	return 0;
}

jnative_t* jnative_find_method(const char* name) {
	assert(name);

	if(!jnative_handlers) {
		list_init(jnative_handlers);
	}

	if(list_empty(jnative_handlers))
		return NULL;

	list_foreach(value, jnative_handlers) {
		jnative_t* cc = (jnative_t*) value;

		if(strcmp(cc->name, name) == 0)
			return cc;
	}

	return NULL;
}
