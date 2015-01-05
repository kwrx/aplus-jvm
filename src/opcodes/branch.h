

OPCODE(goto) {
	int16_t off = PC16;
	PC += 2;

	JGOTO(off);
}

OPCODE(jsr) {
	int16_t off = PC16;
	PC += 2;

	JPUSH(i32, PC);
	JGOTO(off);
}

OPCODE(ret) {
	int8_t idx = PC8;
	PC++;

	PC = j->locals[idx].i32;
}

OPCODE(tableswitch) {
	PC += 3;	/* Padding */
	
	int32_t def = PC32;
	PC += 4;

	int32_t low = PC32;
	PC += 4;

	int32_t high = PC32;
	PC += 4;

	int32_t idx = JPOP(i32);

	if(idx < low || idx > high) {
		JGOTO(def);
	} else {
		PC += ((idx - low) * 4);
		JGOTO(PC32);
	}
}


OPCODE(lookupswitch) {
	PC += 3;	/* Padding */
	
	int32_t def = PC32;
	PC += 4;

	int32_t npairs = PC32;
	PC += 4;

	
	R0 = JPOP_JV();

	assert(npairs >= 0);
	assert(def >= 0);

	int32_t i = 0;
	for(i = 0; i < npairs; i++) {
		R1.i32 = PC32;
		PC += 4;

		if(R1.i32 == R0.i32) {
			JGOTO(PC32);
			return;
		}
		
		PC += 4;
	}

	JGOTO(def);
}


OPCODE(ireturn) {
	R0.i32 = JPOP(i32);
	JRETURN;
}

OPCODE(lreturn) {
	R0.i64 = JPOP(i64);
	JRETURN;
}

OPCODE(freturn) {
	R0.f32 = JPOP(f32);
	JRETURN;
}

OPCODE(dreturn) {
	R0.f64 = JPOP(f64);
	JRETURN;
}

OPCODE(areturn) {
	R0.ptr = JPOP(ptr);
	JRETURN;
}

OPCODE(return) {
	JRETURN;
}




