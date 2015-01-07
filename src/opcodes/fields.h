

OPCODE(getstatic) {
	int16_t idx = PC16;
	PC += 2;

	fieldinfo_t* field = (fieldinfo_t*) jcode_find_fieldref(j->current_assembly, NULL, idx);
	JPUSH_JV(field->value);
}

OPCODE(putstatic) {
	int16_t idx = PC16;
	PC += 2;

	fieldinfo_t* field = (fieldinfo_t*) jcode_find_fieldref(j->current_assembly, NULL, idx);
	field->value = JPOP_JV();
}


OPCODE(getfield) {
	int16_t idx = PC16;
	PC += 2;

	jobject_t* obj = (jobject_t*) JPOP(ptr);

	if(unlikely(!obj))
		j_throw(j, JEXCEPTION_NULL_POINTER);

	fieldinfo_t* field = (fieldinfo_t*) jcode_find_fieldref(j->current_assembly, obj->fields, idx);
	JPUSH_JV(field->value);
}

OPCODE(putfield) {
	int16_t idx = PC16;
	PC += 2;

	jvalue_t jval = JPOP_JV();
	jobject_t* obj = (jobject_t*) JPOP(ptr);


	if(unlikely(!obj))
		j_throw(j, JEXCEPTION_NULL_POINTER);

	fieldinfo_t* field = (fieldinfo_t*) jcode_find_fieldref(j->current_assembly, obj->fields, idx);
	field->value = jval;
}
