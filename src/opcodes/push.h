

OPCODE(bipush) {
	JPUSH(u8, PC8);
	PC++;
}

OPCODE(sipush) {
	JPUSH(u16, PC16);
	PC += 2;
}

