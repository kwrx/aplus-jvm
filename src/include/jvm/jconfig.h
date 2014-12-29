#ifndef _JCONFIG_H
#define _JCONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <sys/times.h>

#define j_printf(s, a...)		\
	printf("[%f] (%s) " s, (double)clock() / CLOCKS_PER_SEC, __func__, a);


#define j_bswap16(x)	\
	__builtin_bswap16(x)

#define j_bswap32(x)	\
	__builtin_bswap32(x)

#define j_bswap64(x)	\
	__builtin_bswap64(x)



static inline void* jmalloc(size_t size) {
	void* p = malloc(size);
	assert(p);

	memset(p, 0, size);
	return p;
}

static inline void jfree(void* ptr) {
	free(ptr);
}



#endif
