

OPCODE(ldc) {
	uint8_t idx = PC8;
	PC++;

	cpvalue_t* v = (cpvalue_t*) list_at_index(j->current_assembly->header.jc_cpinfo, idx - 1);
	jcheck(v);


	switch(v->tag) {
		case JCLASS_TAG_STRING:
			v = (cpvalue_t*) list_at_index(j->current_assembly->header.jc_cpinfo, v->value - 1);
		case JCLASS_TAG_UTF8STRING:
			JPUSH(ptr, (void*) v->data);
			break;
		default:
			JPUSH(i32, v->value);
	}

}

OPCODE(ldc_w) {
	uint16_t idx = PC16;
	PC += 2;

	cpvalue_t* v = (cpvalue_t*) list_at_index(j->current_assembly->header.jc_cpinfo, idx - 1);
	jcheck(v);

	switch(v->tag) {
		case JCLASS_TAG_STRING:
			v = (cpvalue_t*) list_at_index(j->current_assembly->header.jc_cpinfo, v->value - 1);
		case JCLASS_TAG_UTF8STRING:
			JPUSH(ptr, (void*) v->data);
			break;
		default:
			JPUSH(i32, v->value);
	}
}

OPCODE(ldc2_w) {
	uint16_t idx = PC16;
	PC += 2;

	cpvalue_t* v = (cpvalue_t*) list_at_index(j->current_assembly->header.jc_cpinfo, idx - 1);
	jcheck(v);

	JPUSH(i64, v->value);
}
