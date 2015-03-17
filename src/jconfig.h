#ifndef _JCONFIG_H
#define _JCONFIG_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <sched.h>





#define j_throw(j, e)			\
	jerr_set_and_throw(j, e)

#define j_bswap16(x)			\
	__builtin_bswap16(x)

#define j_bswap32(x)			\
	__builtin_bswap32(x)

#define j_bswap64(x)			\
	__builtin_bswap64(x)

#define likely(x)			__builtin_expect((long int) !!(x), 1)
#define unlikely(x)			__builtin_expect((long int) !!(x), 0)





#define jerror(s)							\
	{										\
		jprintf("jvm: ERROR -> %s\n", s);	\
		jexit(-1);							\
	}



#define jcheck(x)																						\
	{																									\
		if(unlikely(!(x))) {																			\
			jprintf("jvm: checking failed on condition (\"%s\") in %s:%d\n", #x, __FILE__, __LINE__);	\
			jexit(-1);																					\
		}																								\
	}




extern void* jmalloc(size_t size);
extern void jfree(void* ptr);

extern void jyield();
extern void jexit(int status);

extern int jprintf(char* str, ...);

int jopen(const char* filename);
int jclose(int fd);
int jread(int fd, void* b, int size);
int jwrite(int fd, void* b, int size);
int jseek(int fd, int p, int dir);




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





#endif
