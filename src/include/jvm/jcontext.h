#ifndef _JCONTEXT_H
#define _JCONTEXT_H

#include "jassembly.h"

typedef union jvalue {
	int8_t i8;
	int16_t i16;
	int32_t i32;
	int64_t i64;

	uint8_t u8;
	uint16_t u16;
	uint32_t u32;
	uint64_t u64;

	float f32;
	double f64;
	
	void* ptr;
} jvalue_t;

typedef struct jcontext {
	list_t* assemblies;
	jassembly_t* current_assembly;

	struct {
		jvalue_t r0;
		jvalue_t r1;
		jvalue_t r2;
		jvalue_t r3;

		uint32_t pc;
		uint32_t fl;
	} regs;


	uint8_t* code;

	jvalue_t* stack;
	uint32_t stack_size;
	uint32_t stack_top;

	jvalue_t* locals;
	uint32_t locals_count;
} jcontext_t;

#endif
