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

typedef struct {
	const char filename[256];
	uint32_t size;
	uint32_t offset;
} jpk_node_t;



#ifdef __JPK__

typedef struct {
	const char* filename;
	int fd;

	char** inputs;
	int inputs_count;
	
	uint32_t offset;
	list_t* files;
} jpk_ctx_t;




int jpk_add_file(jpk_ctx_t* jpk, const char* filename) {
	int fd = open(filename, O_RDONLY, 0644);
	if(fd < 0)
		return -1;

	jpk_node_t* node = (jpk_node_t*) calloc(sizeof(jpk_node_t), 1);
	if(!node)
		return -1;

	strcpy((char*) node->filename, filename);
	
	lseek(fd, 0, SEEK_END);
	node->size = (uint32_t) lseek(fd, 0, SEEK_CUR);
	lseek(fd, 0, SEEK_SET);

	node->offset = jpk->offset;
	jpk->offset += node->size;


	list_add(jpk->files, (listval_t) node);
	close(fd);

	return 0;
}


int main(int argc, char** argv) {
	if(argc < 3)
		exit(1);

	/* jpk [output] [input1, ...] */

	jpk_ctx_t jpk;
	jpk.filename = argv[1];
	jpk.inputs = &argv[2];
	jpk.inputs_count = argc - 2;

	jpk.fd = creat(jpk.filename, 0644);
	if(jpk.fd < 0)
		exit(printf("jpk: cannot open output file\n"));

	list_init(jpk.files);
	
	for(int i = 0; i < jpk.inputs_count; i++)
		jpk_add_file(&jpk, jpk.inputs[i]);
	

	#define W1(x)		{ __t = x; write(jpk.fd, (void*) &__t, 1); }
	#define W2(x)		{ __t = x; write(jpk.fd, (void*) &__t, 2); }
	#define W4(x)		{ __t = x; write(jpk.fd, (void*) &__t, 4); }
	#define W8(x)		{ __t = x; write(jpk.fd, (void*) &__t, 8); }
	#define WX(x, y)	{ write(jpk.fd, (void*) x, y); }

	uint64_t __t;



	/* Magic number */
	W1('J');
	W1('P');
	W1('K');
	W1(0);

	/* Total nodes */
	W4(list_size(jpk.files));

	/* Offset data */
	W4(list_size(jpk.files) * sizeof(jpk_node_t) + 12);

	/* Nodes */
	list_foreach(v, jpk.files)
		WX(v, sizeof(jpk_node_t));
	
	/* Data */
	list_foreach(v, jpk.files) {
		jpk_node_t* node = (jpk_node_t*) v;

		int fd = open(node->filename, O_RDONLY, 0644);
		char buf[64];
		int r = 0;
		while((r = read(fd, buf, sizeof(buf))))
			WX(buf, r);

		close(fd);
	}

	W8(0);

	close(jpk.fd);
	return 0;
}

#else

int jpk_load_memory(void* buffer, size_t size) {
	jcheck(buffer && size);

	if(strncmp((const char*) buffer, "JPK", 3) != 0)
		return -1;

	uint32_t* hdr = (uint32_t*) buffer;
	int nodes_count = hdr[1];
	int offset_data = hdr[2];


	jpk_node_t* nodes = (jpk_node_t*) &hdr[3];
	for(int i = 0; i < nodes_count; i++) {
		jassembly_t* jv;
		jcheck(jassembly_load_memory(&jv, nodes[i].filename, (void*) ((uint32_t) hdr + offset_data + (int) nodes[i].offset), (size_t) nodes[i].size) == 0);
	}

	return 0;
}

int jpk_load(const char* filename) {
	jcheck(filename);

	int fd = jopen(filename);
	if(fd <= 0)
		return -1;

	jseek(fd, 0, SEEK_END);
	size_t size = jseek(fd, 0, SEEK_CUR);
	jseek(fd, 0, SEEK_SET);

	void* buffer = (void*) jmalloc(size);
	jcheck(jread(fd, buffer, size) == size);
	jcheck(jclose(fd) == 0);


	return jpk_load_memory(buffer, size);
}

#endif
