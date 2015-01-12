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

#define _WITH_JDIRS
#include "jconfig.h"


list_t* assemblies_loaded = NULL;


static int jopen(const char* filename, int flags, int mode) {

	
	char* vbuf;
	int i;
	for(i = 0; j_directories[i]; i++) {
		vbuf = (char*) jmalloc(256);
		sprintf(vbuf, j_directories[i], filename);


		int fd = open(vbuf, flags, mode);
		jfree(vbuf);

		if(fd < 0)
			continue;

		return fd;
	}

	return -1;
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

	(*j) = (jassembly_t*) jmalloc(sizeof(jassembly_t));

	(*j)->fd = jopen(filename, O_RDONLY, 0644);
	(*j)->path = (char*) strdup(filename);

	if(unlikely((*j)->fd < 0)) {
		j_printf("could not open \"%s\": %s\n", filename, strerror(errno));
		return -1;
	}

	jcheck(list_add(assemblies_loaded, (listval_t) (*j)) == 0);

	jcheck(jclass_parse_assembly((*j)) == 0);
	jcheck(close((*j)->fd) == 0);
	jcheck(jclass_resolve_assembly((*j)) == 0);

	
	return 0;
}


jassembly_t* jassembly_find(jassembly_t* j, const char* classname) {
	jcheck(j && classname);

	if(strcmp(j->name, classname) == 0)
		return j;

	if(list_empty(j->deps))
		return NULL;

	list_foreach(value, j->deps) {
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


#ifdef TEST
#ifdef DEBUG
static void print_attributes(jassembly_t* j, list_t* attributes) {
	jcheck(j && attributes);

	printf("(");

	list_foreach(value, attributes) {
		attrinfo_t* v = (attrinfo_t*) value;
		cputf8_t utf1;
		jclass_get_utf8_from_cp(j, &utf1, v->name_index);
		
		printf(" %s", utf1.value);
	}

	printf(" )\n");
}
#endif


int main(int argc, char** argv) {

	jcheck(argc > 1);


	jinit();

	jassembly_t* j = NULL;
	jcheck(jassembly_load(&j, argv[1]) == 0);


#ifdef DEBUG

	printf("Loaded %d assemblies:\n", assemblies_loaded->size);
	list_foreach(value, assemblies_loaded) {
		printf("\t%s\n", ((jassembly_t*) value)->name);
	}
	printf("\n");

	printf("Name:\t\t%s (%d)\n", j->name, j->fd);
	printf("Magic:\t\t%8X\n", j->header.jc_magic);
	printf("Minor:\t\t%8d\n", j->header.jc_minor);
	printf("Major:\t\t%8d\n", j->header.jc_major);
	printf("Access:\t\t%8d\n", j->header.jc_access);
	printf("This:\t\t%8d\n", j->header.jc_this);
	printf("Super:\t\t%8d\n", j->header.jc_super);
	printf("Interfaces:\t%8d\n", j->header.jc_interfaces_count);
	printf("Fields:\t\t%8d\n", j->header.jc_fields_count);
	printf("Methods:\t%8d\n", j->header.jc_methods_count);
	printf("Attributes:\t%8d\n", j->header.jc_attributes_count);
	printf("Cpinfo:\t\t%8d\n", j->header.jc_cp_count);
	printf("\n");
	
	int i;

	if(j->header.jc_interfaces_count) {
		printf("Interfaces:\n");
		for(i = 0; i < j->header.jc_interfaces_count; i++)
			printf("\t[%d] => %d\n", i, j->header.jc_interfaces[i]);
		printf("\n");
	}


	if(j->header.jc_fields_count) {
		printf("Fields:\n");
		list_foreach(value, j->header.jc_fields) {
			fieldinfo_t* v = (fieldinfo_t*) value;
			cputf8_t utf1, utf2;
			jclass_get_utf8_from_cp(j, &utf1, v->name_index);
			jclass_get_utf8_from_cp(j, &utf2, v->desc_index);

		
			printf("\t%s\t\t=> %s = %lld (0x%x)\t", utf1.value, utf2.value, v->value.i64, v->value.u32);
		
			print_attributes(j, v->attributes);
		}
		printf("\n");
	}


	if(j->header.jc_methods_count) {
		printf("Methods:\n");
		list_foreach(value, j->header.jc_methods) {
			methodinfo_t* v = (methodinfo_t*) value;
			cputf8_t utf1, utf2;
			jclass_get_utf8_from_cp(j, &utf1, v->name_index);
			jclass_get_utf8_from_cp(j, &utf2, v->desc_index);

			printf("\t%s\t\t=> %s\t", utf1.value, utf2.value);
			print_attributes(j, v->attributes);
		}
		printf("\n");
	}

	if(j->header.jc_attributes_count) {
		printf("Attributes:\n\t");
		print_attributes(j, j->header.jc_attributes);
		printf("\n");
	}

#endif


	jvalue_t ret = jcode_function_call(j, "main", NULL, 0);

	jerr_check_exceptions(NULL);
	j_printf("Returned %d\n", ret.i32);

	return 0;
}
#endif
