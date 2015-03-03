#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include <time.h>
#include <sys/time.h>
#include <sys/times.h>

#define _GNU_SOURCE
#include <float.h>
#include <math.h>


#include <jvm/jvm.h>
#include "jconfig.h"

#define THIS	jobject_t* __this




int JNATIVE(Print, L) (char* s) {
	return printf("Native.Print: %s\n", s);
}

int jinit() {


	#define R(cname, name, desc, ret)	\
		jnative_register_function(cname, #name, #desc, ret, JNATIVE(name, desc))

	R("Native", Print, L, T_INT);
	
	#undef R

	return 0;
}


__attribute__((weak))
#ifdef __aplus__
void
#else
int
#endif
sched_yield() {
	return 
#ifndef __aplus__
		0
#endif
	;
}
