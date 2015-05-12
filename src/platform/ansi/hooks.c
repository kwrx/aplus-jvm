
#ifdef CONFIG_ANSI



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <jvm/jvm.h>
#include "../../jconfig.h"

__weak
int jprintf(char* str, ...) {
	va_list f;
	va_start(f, str);
	int r = vprintf(str, f);
	va_end(f);

	return r;
}

__weak
void jyield() {
	return;
}

__weak
void jexit(int status) {
	exit(status);
}


__weak
void* jmalloc(size_t size) {
	return calloc(size, 1);
}

__weak
void jfree(void* ptr) {
	free(ptr);
}


__weak
int jopen(const char* filename) {
	return (int) fopen(filename, "rb");
}

__weak
int jclose(int fd) {
	return fclose((FILE*) fd);
}

__weak
int jread(int fd, void* b, int size) {
	return fread(b, 1, size, (FILE*) fd);
}

__weak
int jwrite(int fd, void* b, int size) {
	return fwrite(b, 1, size, (FILE*) fd);
}

__weak
int jseek(int fd, int pos, int dir) {
	fseek((FILE*) fd, pos, dir);
	return ftell((FILE*) fd);
}

#endif
