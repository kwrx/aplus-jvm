

OPCODE(new) {

}

OPCODE(newarray) {
	int8_t type = PC8;
	PC++;

	int32_t count = JPOP(i32);

	if(count < 0)
		j_throw(j, JEXCEPTION_NEGATIVE_ARRAY_SIZE);


	jarray_t* arr = (jarray_t*) jmalloc(sizeof(jarray_t));
	if(!__builtin_expect((long int) arr, 0))
		j_throw(j, JEXCEPTION_OUT_OF_MEMORY);


	arr->data = (jvalue_t*) jmalloc(sizeof(jvalue_t) * count);
	if(!__builtin_expect((long int) arr->data, 0))
		j_throw(j, JEXCEPTION_OUT_OF_MEMORY);


	arr->type = type;
	arr->length = count;
	JPUSH(ptr, (void*) arr);
}

OPCODE(anewarray) {
	(void) PC16;
	PC += 2;

	int32_t count = JPOP(i32);

	if(count < 0)
		j_throw(j, JEXCEPTION_NEGATIVE_ARRAY_SIZE);


	jarray_t* arr = (jarray_t*) jmalloc(sizeof(jarray_t));
	if(!__builtin_expect((long int) arr, 0))
		j_throw(j, JEXCEPTION_OUT_OF_MEMORY);


	arr->data = (jvalue_t*) jmalloc(sizeof(jvalue_t) * count);
	if(!__builtin_expect((long int) arr->data, 0))
		j_throw(j, JEXCEPTION_OUT_OF_MEMORY);


	arr->type = T_REFERENCE;
	arr->length = count;
	JPUSH(ptr, (void*) arr);
}
