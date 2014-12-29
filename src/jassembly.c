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


int jassembly_load(jassembly_t* j, const char* filename) {
	assert(j && filename);
	memset(j, 0, sizeof(jassembly_t));

	j->filename = filename;
	j->fd = open(filename, O_RDONLY, 0644);
	
	assert(j->fd > 0);
	assert(jclass_parse_assembly(j) == 0);

	close(j->fd);
	return 0;
}



#ifdef TEST
static void print_attributes(jassembly_t* j, list_t* attributes) {
	assert(j && attributes);

	printf("(");

	list_foreach(value, attributes) {
		attrinfo_t* v = (attrinfo_t*) value;
		cputf8_t utf1;
		jclass_get_utf8_from_cp(j, &utf1, v->name_index);
		
		printf(" %s", utf1.value);
	}

	printf(" )\n");
}



int main(int argc, char** argv) {

	assert(argc > 1);

	jassembly_t j;
	assert(jassembly_load(&j, argv[1]) == 0);


	printf("%s (%d)\n", j.filename, j.fd);
	printf("Magic:\t\t%8X\n", j.header.jc_magic);
	printf("Minor:\t\t%8d\n", j.header.jc_minor);
	printf("Major:\t\t%8d\n", j.header.jc_major);
	printf("Access:\t\t%8d\n", j.header.jc_access);
	printf("This:\t\t%8d\n", j.header.jc_this);
	printf("Super:\t\t%8d\n", j.header.jc_super);
	printf("Interfaces:\t%8d\n", j.header.jc_interfaces_count);
	printf("Fields:\t\t%8d\n", j.header.jc_fields_count);
	printf("Methods:\t%8d\n", j.header.jc_methods_count);
	printf("Attributes:\t%8d\n", j.header.jc_attributes_count);
	printf("Cpinfo:\t\t%8d\n", j.header.jc_cp_count);
	printf("\n");
	
	int i;

	if(j.header.jc_interfaces_count) {
		printf("Interfaces:\n");
		for(i = 0; i < j.header.jc_interfaces_count; i++)
			printf("\t[%d] => %d\n", i, j.header.jc_interfaces[i]);
		printf("\n");
	}


	if(j.header.jc_fields_count) {
		printf("Fields:\n");
		list_foreach(value, j.header.jc_fields) {
			fieldinfo_t* v = (fieldinfo_t*) value;
			cputf8_t utf1, utf2;
			jclass_get_utf8_from_cp(&j, &utf1, v->name_index);
			jclass_get_utf8_from_cp(&j, &utf2, v->desc_index);

			printf("\t%s\t\t=> %s\n", utf1.value, utf2.value);
			print_attributes(&j, v->attributes);
		}
		printf("\n");
	}


	if(j.header.jc_methods_count) {
		printf("Methods:\n");
		list_foreach(value, j.header.jc_methods) {
			methodinfo_t* v = (methodinfo_t*) value;
			cputf8_t utf1, utf2;
			jclass_get_utf8_from_cp(&j, &utf1, v->name_index);
			jclass_get_utf8_from_cp(&j, &utf2, v->desc_index);

			printf("\t%s\t\t=> %s\t", utf1.value, utf2.value);
			print_attributes(&j, v->attributes);
		}
		printf("\n");
	}

	if(j.header.jc_attributes_count) {
		printf("Attributes:\n\t");
		print_attributes(&j, j.header.jc_attributes);
		printf("\n");
	}


	jvalue_t ret = jcode_function_call(&j, "main", NULL, 0);
	j_printf("Returned %d\n", ret.i32);


	return 0;
}
#endif
