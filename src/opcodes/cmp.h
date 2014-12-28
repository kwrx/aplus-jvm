

OPCODE(lcmp) {
	int64_t a = JPOP(i64);
	int64_t b = JPOP(i64);
	int32_t r = 0;

	if(a > b)
		r = 1;
	else if(a < b)
		r = -1;
	else
		r = 0;

	j->regs.fl = r;
	JPUSH(i32, r);
}

OPCODE(fcmpl) {
	float a = JPOP(f32);
	float b = JPOP(f32);
	int32_t r = 0;

	if(a == NAN || b == NAN)
		r = -1;
	else if(a > b)
		r = 1;
	else if(a < b)
		r = -1;
	else
		r = 0;


	j->regs.fl = r;
	JPUSH(i32, r);
}

OPCODE(fcmpg) {
	float a = JPOP(f32);
	float b = JPOP(f32);
	int32_t r = 0;

	if(a == NAN || b == NAN)
		r = 1;
	else if(a > b)
		r = 1;
	else if(a < b)
		r = -1;
	else
		r = 0;

	j->regs.fl = r;
	JPUSH(i32, r);
}

OPCODE(dcmpl) {
	double a = JPOP(f64);
	double b = JPOP(f64);
	int32_t r = 0;

	if(a == NAN || b == NAN)
		r = -1;
	else if(a > b)
		r = 1;
	else if(a < b)
		r = -1;
	else
		r = 0;


	j->regs.fl = r;
	JPUSH(i32, r);
}

OPCODE(dcmpg) {
	double a = JPOP(f64);
	double b = JPOP(f64);
	int32_t r = 0;

	if(a == NAN || b == NAN)
		r = 1;
	else if(a > b)
		r = 1;
	else if(a < b)
		r = -1;
	else
		r = 0;


	j->regs.fl = r;
	JPUSH(i32, r);
}
