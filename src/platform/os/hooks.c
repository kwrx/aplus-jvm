
#ifdef CONFIG_OS



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <jvm/jvm.h>
#include "../../jconfig.h"


__weak
int jprintf(char* str, ...) {
	return 0;
}


__weak
void jyield() {
	return;
}

__weak
void jexit(int status) {
	for(;;);
}


__weak
void* jmalloc(size_t size) {
	static uint32_t ptr = 0x400000;

	void* ret = (void*) ptr;
	memset(ret, 0, size);

	ptr += size;
	return ret;
}

__weak
void jfree(void* ptr) {
	return;
}



__weak
int jopen(const char* filename) {
	return -1;
}

__weak
int jclose(int fd) {
	return -1;
}

__weak
int jread(int fd, void* b, int size) {
	return 0;
}

__weak
int jwrite(int fd, void* b, int size) {
	return 0;
}

__weak
int jseek(int fd, int pos, int dir) {
	return 0;
}

#endif
