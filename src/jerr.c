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


#include "opcodes/opcode.h"



static char* jerr_msg = NULL;
static jcontext_t* jerr_ctx = NULL;

void jerr_set_exception(jcontext_t* j, char* errormsg) {
	while(jerr_msg != NULL && jerr_ctx != NULL)
		j_yield();

	jerr_msg = errormsg;
	jerr_ctx = j;
}

void jerr_clear_exception() {
	jerr_msg = NULL;
	jerr_ctx = NULL;
}

int jerr_has_exception() {
	return jerr_msg ? 1 : 0;
}

int jerr_throw(jcontext_t* j) {
	if(j == NULL) {
		printf("Unhandled exception: %s\n", jerr_msg);

		if(jerr_ctx && jerr_ctx->code) {
			attr_linenumbers_t* ln = (attr_linenumbers_t*) jcode_find_attribute(jerr_ctx->current_assembly, jerr_ctx->method->code->attributes, "LineNumberTable");
			assert(ln);

			int i = 0;
			while(jerr_ctx->regs.pb >= ln->table[i].pc)
				i++;
			i--;

			printf("\tat %s.%s():%d\n", jerr_ctx->current_assembly->name, jerr_ctx->method->name, ln->table[i].line);
			printf("\tat bytecode: [%d+%d] %s\n", jerr_ctx->regs.pb, jerr_ctx->regs.pc - jerr_ctx->regs.pb, j_opcodes[jerr_ctx->code[jerr_ctx->regs.pb]].name);
		
#if defined(VERBOSE)
			printf("\nStackdump:\n");
			printf("Size: %d; Position: %d\n", jerr_ctx->stack_size, jerr_ctx->stack_top);
	
			for(i = 0; i < jerr_ctx->stack_size; i++)
				printf(" [%d] %lld\n", i, jerr_ctx->stack[i].i64);
#endif

		}

		exit(-1);
	} else {
		int i;
		for(i = 0; i < j->method->code->exception_table_length; i++) {
			if(j->regs.pb >= j->method->code->exceptions[i].start_pc && j->regs.pb <= j->method->code->exceptions[i].end_pc) {

				cpvalue_t* cp = (cpvalue_t*) list_at_index(j->current_assembly->header.jc_cpinfo, j->method->code->exceptions[i].catch_type - 1);
				assert(cp);

				cpclass_t cl;
				assert(jclass_cp_to_class(cp, &cl) == 0);

				cputf8_t utf;
				assert(jclass_get_utf8_from_cp(j->current_assembly, &utf, cl.name_index) == 0);


				if(strcmp(utf.value, jerr_msg) == 0) {
					PC = PB = j->method->code->exceptions[i].handler_pc;

					jerr_clear_exception();
					return JERR_HANDLED;
				}
			}
		}
	}

	return JERR_UNHANDLED;
}

int jerr_set_and_throw(jcontext_t* j, char* errormsg) {
	jerr_set_exception(j, errormsg);

	if(jerr_throw(j) == JERR_HANDLED)
		return 0;

	JRETURN_AND_THROW;
}

void jerr_check_exceptions(jcontext_t* j) {
	if(jerr_has_exception()) {
		if(jerr_throw(j) == JERR_HANDLED)
			return;

		JRETURN_AND_THROW;
	}
}


