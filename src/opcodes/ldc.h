

OPCODE(ldc) {
	uint8_t idx = PC8;
	PC++;

	cpvalue_t* v = (cpvalue_t*) list_at_index(j->current_assembly->header.jc_cpinfo, idx - 1);
	assert(v);

	if(v->tag != JCLASS_TAG_UTF8STRING) {
		JPUSH(i32, v->value);
	} else {
		JPUSH(ptr, (void*) v->data);
	}
}

OPCODE(ldc_w) {
	uint16_t idx = PC16;
	PC += 2;

	cpvalue_t* v = (cpvalue_t*) list_at_index(j->current_assembly->header.jc_cpinfo, idx - 1);
	assert(v);

	if(v->tag != JCLASS_TAG_UTF8STRING) {
		JPUSH(i32, v->value);
	} else {
		JPUSH(ptr, (void*) v->data);
	}
}

OPCODE(ldc2_w) {
	uint16_t idx = PC16;
	PC += 2;

	cpvalue_t* v = (cpvalue_t*) list_at_index(j->current_assembly->header.jc_cpinfo, idx - 1);
	assert(v);

	JPUSH(i64, v->value);
}
