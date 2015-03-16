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



#ifdef DEBUG
int JNATIVE(Test, L) (char* s) {
	jprintf("Native call test: %s\n", s);
}
#endif

int jinit() {

	#define R(cname, name, desc, ret)	\
		jnative_register_function(cname, #name, #desc, ret, JNATIVE(name, desc))

#ifdef DEBUG
	R("Debug", Test, L, T_INT);
#endif


	#undef R

	return 0;
}

