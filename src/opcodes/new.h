

OPCODE(new) {

}

OPCODE(newarray) {
	int8_t type = PC8;
	PC++;

	R0.i32 = JPOP(i32);

	if(R0.i32 < 0)
		j_throw(j, JEXCEPTION_NEGATIVE_ARRAY_SIZE);

	switch(type) {
		case T_BOOLEAN:
		case T_CHAR:
		case T_BYTE:
			R1.ptr = (void*) jmalloc(R0.i32 * sizeof(int8_t));
			break;
		case T_FLOAT:
		case T_INT:
			R1.ptr = (void*) jmalloc(R0.i32 * sizeof(int32_t));
			break;
		case T_DOUBLE:
		case T_LONG:
			R1.ptr = (void*) jmalloc(R0.i32 * sizeof(int64_t));
			break;
		case T_SHORT:
			R1.ptr = (void*) jmalloc(R0.i32 * sizeof(int16_t));
			break;
		default:
			j_throw(j, JEXCEPTION_RUNTIME_ERROR);
	}

	if(R1.ptr == NULL)
		j_throw(j, JEXCEPTION_OUT_OF_MEMORY);

	JPUSH(ptr, R1.ptr);
}
