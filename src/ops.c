#include "ops.h"


#if !FREESTANDING
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>

#ifdef __GLIBC__
static void free_stub(void* ptr) {
	return;
}
#endif

static struct avm_ops __ops = {
	.calloc = (void* (*)(int, int)) calloc,
	.free = free_stub,
	.open = (int (*)(const char*, int, int)) open,
	.close = close,
	.lseek = (int (*) (int, int, int)) lseek,
	.read = (int (*) (int, void*, int)) read,
	.yield = (void (*) ()) sched_yield
};

#else

#define O_BINARY	0
#define O_RDONLY	0

#define SEEK_SET	0
#define SEEK_END	2
#define SEEK_CUR	1

static void* calloc_stub(int size, int nmemb) {
	extern int end;
	static long p = &end;
	
	register int k = p;
	p += size * nmemb;
	
	return k;
}

static void free_stub(void* ptr) {
	return;
}

static int open_stub(const char* filename, int flags, int mode) {
	return -1;
}

static int close_stub(int fd) {
	return -1;
}

static int lseek_stub(int fd, int pos, int dir) {
	return -1;
}

static int read_stub(int fd, void* buf, int size) {
	return 0;
}

static void yield_stub() {
	return;
}

static struct avm_ops __ops = {
	.calloc = calloc_stub,
	.free = free_stub,
	.open = open_stub,
	.close = close_stub,
	.lseek = lseek_stub,
	.read = read_stub,
	.yield = yield_stub,
};
#endif

struct avm_ops* avm = &__ops;


#if FREESTANDING
__weak
void* memcpy(void* s1, const void* s2, int size) {
	register char* p1 = s1;
	register char* p2 = s2;
	
	while(size--)
		*p1++ = *p2++;
		
	return s1;
}
__weak
char* strcpy(char* s1, const char* s2) {
	register char* s = s1;
	while(*s2)
		*s1++ = *s2++;
		
	return s;
}
__weak
int strlen(const char* s) {
	register int p = 0;
	while(*s++)
		p++;
		
	return p;
}

__weak
char* strdup(const char* s) {
	char* p = avm->calloc(1, strlen(s));
	strcpy(p, s, strlen(s));
	
	return p;
}
#endif
