#include <avm.h>
#include "ops.h"

#if TEST
#include <stdio.h>
#include <string.h>

#define TEST_ASSEMBLY		"Bubble"
#endif

int main(int argc, char** argv) {
	LOGF(APP_VERSION_FORMAT, APP_VERSION_ARGS);
	
#if TEST

	avm_open("bin/rt.jpk");
	avm_open("test/" TEST_ASSEMBLY ".class");


	avm_begin();
	avm_call(TEST_ASSEMBLY, "main", 0);	
	avm_end();

#else
	/* TODO */
#endif

	return 0;
}
