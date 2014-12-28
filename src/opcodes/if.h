

OPCODE(ifeq) {
	int16_t off = PC16;
	PC += 2;

	int32_t v = JPOP(i32);

	if(v == 0)
		PC += off;
}

OPCODE(ifne) {
	int16_t off = PC16;
	PC += 2;

	int32_t v = JPOP(i32);

	if(v != 0)
		PC += off;
}

OPCODE(iflt) {
	int16_t off = PC16;
	PC += 2;

	int32_t v = JPOP(i32);

	if(v < 0)
		PC += off;
}

OPCODE(ifge) {
	int16_t off = PC16;
	PC += 2;

	int32_t v = JPOP(i32);

	if(v >= 0)
		PC += off;
}

OPCODE(ifgt) {
	int16_t off = PC16;
	PC += 2;

	int32_t v = JPOP(i32);

	if(v > 0)
		PC += off;
}

OPCODE(ifle) {
	int16_t off = PC16;
	PC += 2;

	int32_t v = JPOP(i32);

	if(v <= 0)
		PC += off;
}
