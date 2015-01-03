
OPCODE(irem) {
	int32_t b = JPOP(i32);
	int32_t a = JPOP(i32);

	if(!__builtin_expect((long int) b, 0))
		j_throw(j, JEXCEPTION_DIVISION_BY_ZERO);

	JPUSH(i32, a % b);
}

OPCODE(lrem) {
	int64_t b = JPOP(i64);
	int64_t a = JPOP(i64);

	if(!__builtin_expect((long int) b, 0))
		j_throw(j, JEXCEPTION_DIVISION_BY_ZERO);

	JPUSH(i64, a % b);
}

OPCODE(frem) {
	float b = JPOP(f32);
	float a = JPOP(f32);

	JPUSH(f32, fmod(a, b));
}

OPCODE(drem) {
	double b = JPOP(f64);
	double a = JPOP(f64);

	JPUSH(f64, (double) fmod((float) a, (float) b));
}
