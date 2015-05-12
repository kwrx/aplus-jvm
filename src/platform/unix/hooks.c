
#ifdef CONFIG_UNIX

#ifndef __unix__
#warning "__unix__ undefined"
#endif

#include <unistd.h>
#include <fcntl.h>
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
	sched_yield();
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
	return open(filename, O_RDWR, 0644);
}

__weak
int jclose(int fd) {
	return close(fd);
}

__weak
int jread(int fd, void* b, int size) {
	return read(fd, b, size);
}

__weak
int jwrite(int fd, void* b, int size) {
	return write(fd, b, size);
}

__weak
int jseek(int fd, int pos, int dir) {
	return lseek(fd, pos, dir);
}

#endif