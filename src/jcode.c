#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <float.h>
#include <math.h>

#include <assert.h>

#include <jvm/jconfig.h>
#include <jvm/jclass.h>
#include <jvm/jassembly.h>
#include <jvm/jcontext.h>

#define _WITH_OPCODES
#include "opcodes/opcode.h"



int j_throw(jcontext_t* j, char* exception) {
	printf("Exception! %s\n", exception);
	exit(1);
}

int jcode_run(jcontext_t* j, uint8_t* code, jvalue_t* params, int params_count, jvalue_t* retval) {
	assert(j && code);
	memset(&j->regs, 0, sizeof(j->regs));

	j->code = code;

	while(1) {
		if(j_opcodes[code[j->regs.pc]] == NULL)
			j_throw(j, JEXCEPTION_INVALID_OPCODE);

		j->regs.pc += 1;
		j_opcodes[code[j->regs.pc - 1]] (j);
	}

	return 0;
}
