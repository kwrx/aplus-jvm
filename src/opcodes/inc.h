

OPCODE(iinc) {
	uint8_t idx = PC8;
	PC++;

	int8_t inc = PC8;
	PC++;

	j->locals[idx].i32 += inc;
}
