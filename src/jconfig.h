#ifndef _JCONFIG_H
#define _JCONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <sched.h>

#define j_printf(s, a...)		\
	printf("[%f] (%s) " s, (double)clock() / CLOCKS_PER_SEC, __func__, a);

#define j_yield() 				\
	sched_yield()

#define j_throw(j, e)			\
	jerr_set_and_throw(j, e)

#define j_bswap16(x)			\
	__builtin_bswap16(x)

#define j_bswap32(x)			\
	__builtin_bswap32(x)

#define j_bswap64(x)			\
	__builtin_bswap64(x)

#define j_error(s)							\
	{										\
		printf("jvm: ERROR -> %s\n", s);	\
		exit(-1);							\
	}

#define likely(x)			__builtin_expect((long int) !!(x), 1)
#define unlikely(x)			__builtin_expect((long int) !!(x), 0)

#define jcheck(x)																						\
	{																									\
		if(unlikely(!(x))) {																			\
			printf("jvm: checking failed on condition (\"%s\") in %s:%d\n", #x, __FILE__, __LINE__);	\
			exit(-1);																					\
		}																								\
	}





static inline void* jmalloc(size_t size) {
	void* p = malloc(size);
	if(!__builtin_expect((long int) p, 0))
		return NULL;

	memset(p, 0, size);
	return p;
}

static inline void jfree(void* ptr) {
	free(ptr);
}


#ifdef _WITH_JDIRS
static char* j_directories[] = {
	"%s",
	"%s.class",
	"lib/%s.class",
	"../lib/%s.class",
	"/usr/lib/%s.class",
	"/usr/local/lib/%s.class",

#ifdef TEST
	"bin/lib/%s.class",
#endif

	NULL
};
#endif



#endif
