
OPCODE(idiv) {
	int32_t a = JPOP(i32);
	int32_t b = JPOP(i32);

	if(b == 0)
		j_throw(j, JEXCEPTION_DIVISION_BY_ZERO);

	JPUSH(i32, a / b);
}

OPCODE(ldiv) {
	int64_t a = JPOP(i64);
	int64_t b = JPOP(i64);

	if(b == 0)
		j_throw(j, JEXCEPTION_DIVISION_BY_ZERO);

	JPUSH(i64, a / b);
}

OPCODE(fdiv) {
	float a = JPOP(f32);
	float b = JPOP(f32);

	JPUSH(f32, a / b);
}

OPCODE(ddiv) {
	double a = JPOP(f64);
	double b = JPOP(f64);

	JPUSH(f64, a / b);
}
