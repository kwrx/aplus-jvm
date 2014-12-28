

OPCODE(ldc) {
	uint8_t idx = PC8;
	PC++;

	cpvalue_t* v = (cpvalue_t*) list_at_index(j->current_assembly->header.jc_cpinfo, idx - 1);
	assert(v);

	JPUSH(ptr, v);
}

OPCODE(ldc_w) {
	uint16_t idx = PC16;
	PC += 2;

	cpvalue_t* v = (cpvalue_t*) list_at_index(j->current_assembly->header.jc_cpinfo, idx - 1);
	assert(v);

	JPUSH(ptr, v);
}

OPCODE(ldc2_w) {
	uint16_t idx = PC16;
	PC += 2;

	cpvalue_t* v = (cpvalue_t*) list_at_index(j->current_assembly->header.jc_cpinfo, idx - 1);
	assert(v);

	JPUSH(ptr, v);
}
