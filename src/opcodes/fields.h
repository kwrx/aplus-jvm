

OP(getstatic) {
	int16_t idx = PC16;
	PC += 2;

	cpvalue_t* v = (cpvalue_t*) list_at_index(j->current_assembly->header.jc_cpinfo, idx - 1);
	assert(v);

	JPUSH(ptr, v->value);
}

OP(putstatic) {
	int16_t idx = PC16;
	PC += 2;

	cpvalue_t* v = (cpvalue_t*) list_at_index(j->current_assembly->header.jc_cpinfo, idx - 1);
	assert(v);

	v->value = JPOP(u64);
}
