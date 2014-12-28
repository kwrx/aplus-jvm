

OPCODE(iaload) {
	int32_t* aref = (int32_t*) JPOP(ptr);
	int idx = JPOP(i32);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	JPUSH(i32, aref[idx]);
}

OPCODE(laload) {
	int64_t* aref = (int64_t*) JPOP(ptr);
	int idx = JPOP(i32);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	JPUSH(i64, aref[idx]);
}

OPCODE(faload) {
	float* aref = (float*) JPOP(ptr);
	int idx = JPOP(i32);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	JPUSH(f32, aref[idx]);
}

OPCODE(daload) {
	double* aref = (double*) JPOP(ptr);
	int idx = JPOP(i32);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	JPUSH(f64, aref[idx]);
}

OPCODE(aaload) {
	void** aref = (void**) JPOP(ptr);
	int idx = JPOP(i32);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	JPUSH(ptr, aref[idx]);
}

OPCODE(baload) {
	uint8_t* aref = (uint8_t*) JPOP(ptr);
	int idx = JPOP(i32);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	JPUSH(u8, aref[idx]);
}

OPCODE(caload) {
	int8_t* aref = (int8_t*) JPOP(ptr);
	int idx = JPOP(i32);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	JPUSH(i8, aref[idx]);
}

OPCODE(saload) {
	int16_t* aref = (int16_t*) JPOP(ptr);
	int idx = JPOP(i32);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	JPUSH(i16, aref[idx]);
}


OPCODE(iastore) {
	int32_t* aref = (int32_t*) JPOP(ptr);
	int idx = JPOP(i32);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	aref[idx] = JPOP(i32);
}

OPCODE(lastore) {
	int64_t* aref = (int64_t*) JPOP(ptr);
	int idx = JPOP(i32);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	aref[idx] = JPOP(i64);
}

OPCODE(fastore) {
	float* aref = (float*) JPOP(ptr);
	int idx = JPOP(i32);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	aref[idx] = JPOP(f32);
}

OPCODE(dastore) {
	double* aref = (double*) JPOP(ptr);
	int idx = JPOP(i32);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	aref[idx] = JPOP(f64);
}

OPCODE(aastore) {
	void** aref = (void**) JPOP(ptr);
	int idx = JPOP(i32);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	aref[idx] = JPOP(ptr);
}

OPCODE(bastore) {
	uint8_t* aref = (uint8_t*) JPOP(ptr);
	int idx = JPOP(i32);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	aref[idx] = JPOP(u8);
}

OPCODE(castore) {
	int8_t* aref = (int8_t*) JPOP(ptr);
	int idx = JPOP(i32);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	aref[idx] = JPOP(i8);
}

OPCODE(sastore) {
	int16_t* aref = (int16_t*) JPOP(ptr);
	int idx = JPOP(i32);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	aref[idx] = JPOP(i16);
}
