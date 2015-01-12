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
			abort();																					\
		}																								\
	}





static inline void* jmalloc(size_t size) {
	void* p = malloc(size);
	if(unlikely(!p))
		return NULL;

	memset(p, 0, size);
	return p;
}

static inline void jfree(void* ptr) {
	free(ptr);
}

static inline void** jarray_from_list(list_t* lst) {
	jcheck(lst);
	
	int32_t* arr = (int32_t*) jmalloc((sizeof(void*) * lst->size) + (sizeof(int32_t) * 3));
	if(unlikely(!arr))
		return NULL;

	*arr++ = (int32_t) JARRAY_MAGICVALUE;
	*arr++ = (int32_t) T_REFERENCE;
	*arr++ = (int32_t) lst->size;
	

	register int i = 0;
	list_foreach(v, lst) {
		((void**) arr) [i++] = (void*) v;
	}

	return (void**) arr;
}

static inline void** jarray_from_ptr(void* ptr, size_t membersize, size_t count) {
	jcheck(ptr && membersize && count);
	
	int32_t* arr = (int32_t*) jmalloc((sizeof(void*) * count) + (sizeof(int32_t) * 3));
	if(unlikely(!arr))
		return NULL;

	*arr++ = (int32_t) JARRAY_MAGICVALUE;
	*arr++ = (int32_t) T_REFERENCE;
	*arr++ = (int32_t) count;
	

	register int i;
	for(i = 0; i < count; i++) {
		switch(membersize) {
			case sizeof(uint8_t):
				arr[i] = (int32_t) &((uint8_t*) ptr) [i];
				break;
			case sizeof(uint16_t):
				arr[i] = (int32_t) &((uint16_t*) ptr) [i];
				break;
			case sizeof(uint32_t):
				arr[i] = (int32_t) &((uint32_t*) ptr) [i];
				break;
			case sizeof(uint64_t):
				arr[i] = (int32_t) &((uint64_t*) ptr) [i];
				break;
			default:
				return NULL;
		}
	}

	return (void**) arr;
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
