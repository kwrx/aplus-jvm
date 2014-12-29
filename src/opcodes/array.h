

OPCODE(iaload) {
	int idx = JPOP(i32);
	int32_t* aref = (int32_t*) JPOP(ptr);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	JPUSH(i32, aref[idx]);
}

OPCODE(laload) {
	int idx = JPOP(i32);
	int64_t* aref = (int64_t*) JPOP(ptr);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	JPUSH(i64, aref[idx]);
}

OPCODE(faload) {
	int idx = JPOP(i32);
	float* aref = (float*) JPOP(ptr);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	JPUSH(f32, aref[idx]);
}

OPCODE(daload) {
	int idx = JPOP(i32);
	double* aref = (double*) JPOP(ptr);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	JPUSH(f64, aref[idx]);
}

OPCODE(aaload) {
	int idx = JPOP(i32);
	void** aref = (void**) JPOP(ptr);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	JPUSH(ptr, aref[idx]);
}

OPCODE(baload) {
	int idx = JPOP(i32);
	uint8_t* aref = (uint8_t*) JPOP(ptr);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	JPUSH(u8, aref[idx]);
}

OPCODE(caload) {
	int idx = JPOP(i32);
	int8_t* aref = (int8_t*) JPOP(ptr);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	JPUSH(i8, aref[idx]);
}

OPCODE(saload) {
	int idx = JPOP(i32);
	int16_t* aref = (int16_t*) JPOP(ptr);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	JPUSH(i16, aref[idx]);
}


OPCODE(iastore) {
	R0.i32 = JPOP(i32);
	int idx = JPOP(i32);
	int32_t* aref = (int32_t*) JPOP(ptr);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	aref[idx] = R0.i32;
}

OPCODE(lastore) {
	R0.i64 = JPOP(i64);
	int idx = JPOP(i32);
	int64_t* aref = (int64_t*) JPOP(ptr);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	aref[idx] = R0.i64;
}

OPCODE(fastore) {
	R0.f32 = JPOP(f32);
	int idx = JPOP(i32);
	float* aref = (float*) JPOP(ptr);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	aref[idx] = R0.f32;
}

OPCODE(dastore) {
	R0.f64 = JPOP(f64);
	int idx = JPOP(i32);
	double* aref = (double*) JPOP(ptr);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	aref[idx] = R0.f64;
}

OPCODE(aastore) {
	R0.ptr = JPOP(ptr);
	int idx = JPOP(i32);
	void** aref = (void**) JPOP(ptr);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	aref[idx] = R0.ptr;
}

OPCODE(bastore) {
	R0.u8 = JPOP(u8);
	int idx = JPOP(i32);
	uint8_t* aref = (uint8_t*) JPOP(ptr);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	aref[idx] = R0.u8;
}

OPCODE(castore) {
	R0.i8 = JPOP(i8);
	int idx = JPOP(i32);
	int8_t* aref = (int8_t*) JPOP(ptr);

	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	aref[idx] = R0.i8;
}

OPCODE(sastore) {
	R0.i16 = JPOP(i16);
	int idx = JPOP(i32);
	int16_t* aref = (int16_t*) JPOP(ptr);
	
	if(aref == NULL)
		j_throw(j, JEXCEPTION_NULL_POINTER);

	aref[idx] = R0.i16;
}
