#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <float.h>
#include <math.h>

#include <assert.h>


#include <jvm/jvm.h>
#include "jconfig.h"


int jinit() {
	jnative_register_function("clone", "L", T_REFERENCE, jobject_clone);
	jnative_register_function("finalize", "L", T_VOID, jobject_finalize);

	return 0;
}
