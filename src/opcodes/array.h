

OPCODE(iaload) {
	int32_t idx = JPOP(i32);
	int32_t* aref = (int32_t*) JPOP(ptr);
	
	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);


#if HAVE_ARRAY_BOUNDS_CHECKING
	if(idx >= aref[-1])
		j_throw(j, JEXCEPTION_ARRAY_BOUNDS);
#endif

	JPUSH(i32, aref[idx]);
}

OPCODE(laload) {
	int32_t idx = JPOP(i32);
	int32_t* aref = (int32_t*) JPOP(ptr);
	
	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);


#if HAVE_ARRAY_BOUNDS_CHECKING
	if(idx >= aref[-1])
		j_throw(j, JEXCEPTION_ARRAY_BOUNDS);
#endif

	JPUSH(i64, ((int64_t*) aref) [idx]);
}

OPCODE(faload) {
	int32_t idx = JPOP(i32);
	int32_t* aref = (int32_t*) JPOP(ptr);
	
	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);


#if HAVE_ARRAY_BOUNDS_CHECKING
	if(idx >= aref[-1])
		j_throw(j, JEXCEPTION_ARRAY_BOUNDS);
#endif

	JPUSH(f32, ((float*) aref) [idx]);
}

OPCODE(daload) {
	int32_t idx = JPOP(i32);
	int32_t* aref = (int32_t*) JPOP(ptr);
	
	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);


#if HAVE_ARRAY_BOUNDS_CHECKING
	if(idx >= aref[-1])
		j_throw(j, JEXCEPTION_ARRAY_BOUNDS);
#endif

	JPUSH(f64, ((double*) aref) [idx]);
}

OPCODE(aaload) {
	int32_t idx = JPOP(i32);
	int32_t* aref = (int32_t*) JPOP(ptr);
	
	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);


#if HAVE_ARRAY_BOUNDS_CHECKING
	if(idx >= aref[-1])
		j_throw(j, JEXCEPTION_ARRAY_BOUNDS);
#endif

	JPUSH(ptr, ((void**) aref) [idx]);
}

OPCODE(baload) {
	int32_t idx = JPOP(i32);
	int32_t* aref = (int32_t*) JPOP(ptr);
	
	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);


#if HAVE_ARRAY_BOUNDS_CHECKING
	if(idx >= aref[-1])
		j_throw(j, JEXCEPTION_ARRAY_BOUNDS);
#endif


	JPUSH(u8, ((uint8_t*) aref) [idx]);
}

OPCODE(caload) {
	int32_t idx = JPOP(i32);
	int32_t* aref = (int32_t*) JPOP(ptr);
	
	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);


#if HAVE_ARRAY_BOUNDS_CHECKING
	if(idx >= aref[-1])
		j_throw(j, JEXCEPTION_ARRAY_BOUNDS);
#endif

	JPUSH(i8, ((int8_t*) aref) [idx]);
}

OPCODE(saload) {
	int32_t idx = JPOP(i32);
	int32_t* aref = (int32_t*) JPOP(ptr);
	
	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);


#if HAVE_ARRAY_BOUNDS_CHECKING
	if(idx >= aref[-1])
		j_throw(j, JEXCEPTION_ARRAY_BOUNDS);
#endif

	JPUSH(i16, ((int16_t*) aref) [idx]);
}




OPCODE(iastore) {
	jvalue_t v = JPOP_JV();
	int32_t idx = JPOP(i32);
	int32_t* aref = (int32_t*) JPOP(ptr);
	
	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);


#if HAVE_ARRAY_BOUNDS_CHECKING
	if(idx >= aref[-1])
		j_throw(j, JEXCEPTION_ARRAY_BOUNDS);
#endif

	aref[idx] = v.i32;
}

OPCODE(lastore) {
	jvalue_t v = JPOP_JV();
	int32_t idx = JPOP(i32);
	int32_t* aref = (int32_t*) JPOP(ptr);
	
	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);


#if HAVE_ARRAY_BOUNDS_CHECKING
	if(idx >= aref[-1])
		j_throw(j, JEXCEPTION_ARRAY_BOUNDS);
#endif

	((int64_t*) aref) [idx] = v.i64;
}

OPCODE(fastore) {
	jvalue_t v = JPOP_JV();
	int32_t idx = JPOP(i32);
	int32_t* aref = (int32_t*) JPOP(ptr);
	
	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);


#if HAVE_ARRAY_BOUNDS_CHECKING
	if(idx >= aref[-1])
		j_throw(j, JEXCEPTION_ARRAY_BOUNDS);
#endif

	((float*) aref) [idx] = v.f32;
}

OPCODE(dastore) {
	jvalue_t v = JPOP_JV();
	int32_t idx = JPOP(i32);
	int32_t* aref = (int32_t*) JPOP(ptr);
	
	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);


#if HAVE_ARRAY_BOUNDS_CHECKING
	if(idx >= aref[-1])
		j_throw(j, JEXCEPTION_ARRAY_BOUNDS);
#endif

	((double*) aref) [idx] = v.f64;
}

OPCODE(aastore) {
	jvalue_t v = JPOP_JV();
	int32_t idx = JPOP(i32);
	int32_t* aref = (int32_t*) JPOP(ptr);
	
	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);

#if HAVE_ARRAY_BOUNDS_CHECKING
	if(idx >= aref[-1])
		j_throw(j, JEXCEPTION_ARRAY_BOUNDS);
#endif

	((void**) aref) [idx] = v.ptr;
}

OPCODE(bastore) {
	jvalue_t v = JPOP_JV();
	int32_t idx = JPOP(i32);
	int32_t* aref = (int32_t*) JPOP(ptr);

	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);


#if HAVE_ARRAY_BOUNDS_CHECKING
	if(idx >= aref[-1])
		j_throw(j, JEXCEPTION_ARRAY_BOUNDS);
#endif

	((uint8_t*) aref) [idx] = v.u8;
}

OPCODE(castore) {
	jvalue_t v = JPOP_JV();
	int32_t idx = JPOP(i32);
	int32_t* aref = (int32_t*) JPOP(ptr);
	
	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);


#if HAVE_ARRAY_BOUNDS_CHECKING
	if(idx >= aref[-1])
		j_throw(j, JEXCEPTION_ARRAY_BOUNDS);
#endif

	((int8_t*) aref) [idx] = v.i8;
}

OPCODE(sastore) {
	jvalue_t v = JPOP_JV();
	int32_t idx = JPOP(i32);
	int32_t* aref = (int32_t*) JPOP(ptr);
	
	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);


#if HAVE_ARRAY_BOUNDS_CHECKING
	if(idx >= aref[-1])
		j_throw(j, JEXCEPTION_ARRAY_BOUNDS);
#endif

	((int16_t*) aref) [idx] = v.i16;
}

OPCODE(arraylength) {
	int32_t* aref = (int32_t*) JPOP(ptr);

	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);

	JPUSH(i32, aref[-1]);
}

