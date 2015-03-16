
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


int jprintf(char* str, ...) {
	va_list f;
	va_start(f, str);
	int r = vprintf(str, f);
	va_end(f);

	return r;
}


void jyield() {
	sched_yield();
}

void jexit(int status) {
	exit(status);
}


void* jmalloc(size_t size) {
	return calloc(size, 1);
}

void jfree(void* ptr) {
	free(ptr);
}



int jopen(const char* filename) {
	return open(filename, O_RDWR, 0644);
}

int jclose(int fd) {
	return close(fd);
}

int jread(int fd, void* b, int size) {
	return read(fd, b, size);
}

int jwrite(int fd, void* b, int size) {
	return write(fd, b, size);
}

int jseek(int fd, int pos, int dir) {
	return lseek(fd, pos, dir);
}

#endif
