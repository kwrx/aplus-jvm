#include <avm.h>
#include "ops.h"

typedef struct {
	const char filename[256];
	u4 size;
	u4 offset;
} jpk_node_t;



#ifdef __JPK__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct {
	const char* filename;
	int fd;

	char** inputs;
	int inputs_count;
	
	uint32_t offset;
	jpk_node_t** files;
} jpk_ctx_t;




int jpk_add_file(jpk_ctx_t* jpk, const char* filename, int i) {
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


	jpk->files[i] = node;
	close(fd);


	//printf("\t> \"%s\" (%d Bytes)\n", node->filename, node->size);

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


	jpk.files = (jpk_node_t**) calloc(sizeof(jpk_node_t*), jpk.inputs_count);
	
	int i;
	for(i = 0; i < jpk.inputs_count; i++)
		jpk_add_file(&jpk, jpk.inputs[i], i);
	

	#define W1(x)		{ __t = x; (void) write(jpk.fd, (void*) &__t, 1); }
	#define W2(x)		{ __t = x; (void) write(jpk.fd, (void*) &__t, 2); }
	#define W4(x)		{ __t = x; (void) write(jpk.fd, (void*) &__t, 4); }
	#define W8(x)		{ __t = x; (void) write(jpk.fd, (void*) &__t, 8); }
	#define WX(x, y)	{ (void) write(jpk.fd, (void*) x, y); }

	uint64_t __t;



	/* Magic number */
	W1('J');
	W1('P');
	W1('K');
	W1(0);

	/* Total nodes */
	W4(jpk.inputs_count);

	/* Offset data */
	W4(jpk.inputs_count * sizeof(jpk_node_t) + 12);

	/* Nodes */
	for(i = 0; i < jpk.inputs_count; i++)
		WX(jpk.files[i], sizeof(jpk_node_t));
	
	/* Data */
	for(i = 0; i < jpk.inputs_count; i++) {

		int fd = open(jpk.files[i]->filename, O_RDONLY, 0644);
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


#if !FREESTANDING
#include <fcntl.h>
#include <string.h>
#endif

int jpk_load(void* buffer, size_t size) {

	if(strncmp((const char*) buffer, "JPK", 3) != 0)
		return J_ERR;

	u4* hdr = (u4*) buffer;
	int nodes_count = hdr[1];
	int offset_data = hdr[2];


	int i;
	jpk_node_t* nodes = (jpk_node_t*) &hdr[3];
	for(i = 0; i < nodes_count; i++)
		if(java_assembly_load(NULL, (void*) ((long) hdr + offset_data + (long) nodes[i].offset), (u4) nodes[i].size, nodes[i].filename) != J_OK)
			LOGF("Cannot load %s from JPK Archive", nodes[i].filename);
	

	return J_OK;
}

int jpk_open(const char* filename) {

	int fd = avm->open(filename, O_RDONLY, 0644);
	if(fd < 0)
		return J_ERR;

	int size = avm->lseek(fd, 0, SEEK_END);
	avm->lseek(fd, 0, SEEK_SET);

	void* buffer = (void*) avm->calloc(1, size);
	if(avm->read(fd, buffer, size) != size) {
		LOGF("I/O Read error of %s", filename);

		avm->free(buffer);
		return J_ERR;
	}

	avm->close(fd);


	return jpk_load(buffer, size);
}

#endif

