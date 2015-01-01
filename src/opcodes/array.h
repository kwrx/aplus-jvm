

OPCODE(iaload) {
	int32_t idx = JPOP(i32);
	jarray_t* aref = (jarray_t*) JPOP(ptr);
	
	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);

#if HAVE_ARRAY_BOUNDS_CHECKING
	if(idx >= aref->length)
		j_throw(j, JEXCEPTION_ARRAY_BOUNDS);
#endif

	JPUSH_JV(aref->data[idx]);
}

#define j_op_laload	j_op_iaload
#define j_op_faload	j_op_iaload
#define j_op_daload	j_op_iaload
#define j_op_aaload	j_op_iaload
#define j_op_baload	j_op_iaload
#define j_op_caload	j_op_iaload
#define j_op_saload	j_op_iaload



OPCODE(iastore) {
	jvalue_t v = JPOP_JV();
	int32_t idx = JPOP(i32);
	jarray_t* aref = (jarray_t*) JPOP(ptr);

	if(!__builtin_expect((long int) aref, 0))
		j_throw(j, JEXCEPTION_NULL_POINTER);

#if HAVE_ARRAY_BOUNDS_CHECKING
	if(idx >= aref->length)
		j_throw(j, JEXCEPTION_ARRAY_BOUNDS);
#endif

	aref->data[idx] = v;
}

#define j_op_lastore	j_op_iastore
#define j_op_fastore	j_op_iastore
#define j_op_dastore	j_op_iastore
#define j_op_aastore	j_op_iastore
#define j_op_bastore	j_op_iastore
#define j_op_castore	j_op_iastore
#define j_op_sastore	j_op_iastore

