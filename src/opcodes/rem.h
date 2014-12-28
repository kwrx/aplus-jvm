
OPCODE(irem) {
	int32_t a = JPOP(i32);
	int32_t b = JPOP(i32);

	if(b == 0)
		j_throw(j, JEXCEPTION_DIVISION_BY_ZERO);

	JPUSH(i32, a % b);
}

OPCODE(lrem) {
	int64_t a = JPOP(i64);
	int64_t b = JPOP(i64);

	if(b == 0)
		j_throw(j, JEXCEPTION_DIVISION_BY_ZERO);

	JPUSH(i64, a % b);
}

OPCODE(frem) {
	float a = JPOP(f32);
	float b = JPOP(f32);

	JPUSH(f32, fmod(a, b));
}

OPCODE(drem) {
	double a = JPOP(f64);
	double b = JPOP(f64);

	JPUSH(f64, (double) fmod((float) a, (float) b));
}
