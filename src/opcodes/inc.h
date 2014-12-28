

OPCODE(iinc) {
	uint8_t idx = PC8;
	PC++;

	int8_t inc = PC8;
	PC++;

	assert(idx < j->locals_count);
	j->locals[idx].i64 += inc;
}
