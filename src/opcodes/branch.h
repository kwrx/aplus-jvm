

OPCODE(goto) {
	int16_t off = PC16;
	PC += 2;

	PC = off;
}

OPCODE(jsr) {
	int16_t off = PC16;
	PC += 2;

	JPUSH(i32, PC);
	PC = off;
}

OPCODE(ret) {
	uint8_t idx = PC8;
	PC++;

	assert(idx < j->locals_count);
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
	int32_t i = 0;
	int32_t off = 0;

	if(idx < low || idx > high)
		off = def;
	else {
		for(i = 0; i < (idx - low); i++) {
			off = PC32;
			PC += 4;
		}
	}

	PC = off;
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
			PC = PC32;
			return;
		}
		
		PC += 4;
	}

	PC = def;
}


OPCODE(ireturn) {
	assert(0 && "Istruction not yet supported");
}

OPCODE(lreturn) {
	assert(0 && "Istruction not yet supported");
}

OPCODE(freturn) {
	assert(0 && "Istruction not yet supported");
}

OPCODE(dreturn) {
	assert(0 && "Istruction not yet supported");
}

OPCODE(areturn) {
	assert(0 && "Istruction not yet supported");
}

OPCODE(return) {
	assert(0 && "Istruction not yet supported");
}




