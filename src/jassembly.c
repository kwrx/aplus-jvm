#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define _GNU_SOURCE
#include <float.h>
#include <math.h>

#include <jvm/jvm.h>


#include "jconfig.h"


list_t* assemblies_loaded = NULL;



int jassembly_load_memory(jassembly_t** j, const char* filename, void* buffer, size_t size) {
	if(assemblies_loaded == NULL)
		{ list_init(assemblies_loaded); }

	jcheck(filename);

	list_foreach(value, assemblies_loaded) {
		jassembly_t* jv = (jassembly_t*) value;

		if(strcmp(jv->path, filename) == 0) {
			if(j)
				(*j) = jv;
			return 0;
		}
	}

	 
	jassembly_t* jv = (jassembly_t*) jmalloc(sizeof(jassembly_t));
	jv->path = (char*) strdup(filename);
	jv->buffer = buffer;
	jv->size = size;
	jv->resolved = 0;
	

	jcheck(list_add(assemblies_loaded, (listval_t) jv) == 0);
	jcheck(jclass_parse_assembly(jv) == 0);

	if(j)
		(*j) = jv;
	
	return 0;
}

int jassembly_load(jassembly_t** j, const char* filename) {

	if(assemblies_loaded == NULL)
		{ list_init(assemblies_loaded); }

	jcheck(j && filename);


	list_foreach(value, assemblies_loaded) {
		jassembly_t* jv = (jassembly_t*) value;

		if(strcmp(jv->path, filename) == 0) {
			(*j) = jv;
			return 0;
		}
	}

	

	int fd = jopen(filename);
	if(unlikely(fd < 0)) {
		jprintf("could not open \"%s\": %s\n", filename, strerror(errno));
		return -1;
	}


	jseek(fd, 0, SEEK_END);
	size_t size = jseek(fd, 0, SEEK_CUR);
	jseek(fd, 0, SEEK_SET);


	void* buffer = (void*) jmalloc(size);
	if(unlikely(!buffer)) {
		jprintf("could not allocate memory for \"%s\": %s\n", filename, strerror(errno));
		return -1;
	}


	jcheck(jread(fd, buffer, size) == size);
	jcheck(jclose(fd) == 0);

	return jassembly_load_memory(j, filename, buffer, size);
}


jassembly_t* jassembly_find(jassembly_t* j, const char* classname) {
	jcheck(classname);

	if((unlikely(!j)))
		j = (jassembly_t*) list_tail(assemblies_loaded);
	
	jcheck(j);

	if(strcmp(j->name, classname) == 0)
		return j;


	list_foreach(value, j->deps) {
		jassembly_t* cc = (jassembly_t*) value;

		if(strcmp(cc->name, classname) == 0)
			return cc;
	}

	list_foreach(value, assemblies_loaded) {
		jassembly_t* cc = (jassembly_t*) value;

		if(strcmp(cc->name, classname) == 0)
			return cc;
	}

	return NULL;
}


int jassembly_change(jcontext_t* j, const char* classname) {
	jcheck(j && classname);

	j->last_assembly = j->current_assembly;
	j->current_assembly = jassembly_find(j->current_assembly, classname);

	jcheck(j->current_assembly);
	return 0;
}

int jassembly_restore(jcontext_t* j) {
	jcheck(j);

	if(j->last_assembly)
		j->current_assembly = j->last_assembly;

	return 0;
}


int jassembly_resolve_all() {
	list_foreach(value, assemblies_loaded) {
		jassembly_t* jv = (jassembly_t*) value;

		if(!jv->resolved)
			jcheck(jclass_resolve_assembly(jv) == 0); 
	}

	return 0;
}



#ifdef TEST
int main(int argc, char** argv) {

	jcheck(argc > 1);


	jinit();

	jassembly_t* j = NULL;
	jcheck(jassembly_load(&j, argv[1]) == 0);


	jcheck(jpk_load("rt.jpk") == 0);
	jcheck(jassembly_resolve_all() == 0);


#ifdef DEBUG

	jprintf("Loaded %d assemblies:\n", assemblies_loaded->size);
	list_foreach(value, assemblies_loaded) {
		jprintf("\t%s\n", ((jassembly_t*) value)->name);
	}
	jprintf("\n");

#endif


	jvalue_t ret = jcode_function_call(j, "main", NULL, 0);
	jerr_check_exceptions(NULL);


	jprintf("Returned %lld\n", ret.i64);

	return 0;
}
#endif
