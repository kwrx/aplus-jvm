

OPCODE(new) {

}

OPCODE(newarray) {
	int8_t type = PC8;
	PC++;

	int32_t count = JPOP(i32);

	if(count < 0)
		j_throw(j, JEXCEPTION_NEGATIVE_ARRAY_SIZE);


	register int sz;
	switch(type) {
		case T_BOOLEAN:
		case T_CHAR:
		case T_BYTE:
			sz = sizeof(int8_t);
			break;
		case T_SHORT:
			sz = sizeof(int16_t);
			break;
		case T_INT:
		case T_FLOAT:
		case T_REFERENCE:
			sz = sizeof(int32_t);
			break;
		case T_LONG:
		case T_DOUBLE:
			sz = sizeof(int64_t);
			break;
		default:
			j_throw(j, JEXCEPTION_INVALID_TYPE " of array");
	}

	
	int32_t* arr = (int32_t*) jmalloc((sz * count) + (sizeof(int32_t) * 3));
	*arr++ = (int32_t) JARRAY_MAGIC;
	*arr++ = (int32_t) type;
	*arr++ = (int32_t) count;
	
	JPUSH(ptr, (void*) arr);
}

OPCODE(anewarray) {
	(void) PC16;
	PC += 2;

	int32_t count = JPOP(i32);

	if(count < 0)
		j_throw(j, JEXCEPTION_NEGATIVE_ARRAY_SIZE);


	int32_t* arr = (int32_t*) jmalloc((sizeof(void*) * count) + (sizeof(int32_t) << 1));
	*arr++ = (int32_t) T_REFERENCE;
	*arr++ = (int32_t) count;
	
	JPUSH(ptr, (void*) arr);
}
