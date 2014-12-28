#ifndef _OPCODE_H
#define _OPCODE_H


#include <math.h>

#define JPUSH(t, v)	\
	{ j->stack[j->stack_top++].t = (v); }

#define JPUSH_JV(v)	\
	{ j->stack[j->stack_top++] = (v); }

#define JPOP(t)		\
	j->stack[j->stack_top--].t

#define JPOP_JV()	\
	j->stack[j->stack_top--]

#define OPCODE(n)	\
	static inline void j_op_##n (jcontext_t* j)

#define OP(n)		\
	j_op_##n


#define PC			j->regs.pc
#define PC8			j->code[j->regs.pc]
#define PC16		j_bswap16(*(uint16_t*) (&j->code[j->regs.pc]))
#define PC32		j_bswap32(*(uint32_t*) (&j->code[j->regs.pc]))
#define PC64		j_bswap64(*(uint64_t*) (&j->code[j->regs.pc]))

#define R0			j->regs.r0
#define R1			j->regs.r1
#define R2			j->regs.r2
#define R3			j->regs.r3


#include "nop.h"
#include "const.h"
#include "ldc.h"
#include "push.h"
#include "locals.h"
#include "array.h"
#include "stack.h"
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"
#include "rem.h"
#include "neg.h"
#include "shift.h"
#include "and.h"
#include "or.h"
#include "xor.h"
#include "inc.h"
#include "conv.h"
#include "cmp.h"
#include "if.h"
#include "branch.h"
#include "fields.h"


typedef void (*opcode_handler_t) (jcontext_t*);

opcode_handler_t j_opcodes[255] = {
	OP(nop),
	OP(aconst_null),
	OP(iconst_m1),
	OP(iconst_0),
	OP(iconst_1),
	OP(iconst_2),
	OP(iconst_3),
	OP(iconst_4),
	OP(iconst_5),
	OP(lconst_0),
	OP(lconst_1),
	OP(fconst_0),
	OP(fconst_1),
	OP(fconst_2),
	OP(dconst_0),
	OP(dconst_1),
	OP(bipush),
	OP(sipush),
	OP(ldc),
	OP(ldc_w),
	OP(ldc2_w),
	OP(iload),
	OP(lload),
	OP(fload),
	OP(dload),
	OP(aload),
	OP(iload_0),
	OP(iload_1),
	OP(iload_2),
	OP(iload_3),
	OP(lload_0),
	OP(lload_1),
	OP(lload_2),
	OP(lload_3),
	OP(fload_0),
	OP(fload_1),
	OP(fload_2),
	OP(fload_3),
	OP(dload_0),
	OP(dload_1),
	OP(dload_2),
	OP(dload_3),
	OP(aload_0),
	OP(aload_1),
	OP(aload_2),
	OP(aload_3),
	OP(iaload),
	OP(laload),
	OP(faload),
	OP(daload),
	OP(aaload),
	OP(baload),
	OP(caload),
	OP(saload),
	OP(istore),
	OP(lstore),
	OP(fstore),
	OP(dstore),
	OP(astore),
	OP(istore_0),
	OP(istore_1),
	OP(istore_2),
	OP(istore_3),
	OP(lstore_0),
	OP(lstore_1),
	OP(lstore_2),
	OP(lstore_3),
	OP(fstore_0),
	OP(fstore_1),
	OP(fstore_2),
	OP(fstore_3),
	OP(dstore_0),
	OP(dstore_1),
	OP(dstore_2),
	OP(dstore_3),
	OP(astore_0),
	OP(astore_1),
	OP(astore_2),
	OP(astore_3),
	OP(iastore),
	OP(lastore),
	OP(fastore),
	OP(dastore),
	OP(aastore),
	OP(bastore),
	OP(castore),
	OP(sastore),
	OP(pop),
	OP(pop2),
	OP(dup),
	OP(dup_x1),
	OP(dup_x2),
	OP(dup2),
	OP(dup2_x1),
	OP(dup2_x2),
	OP(iadd),
	OP(ladd),
	OP(fadd),
	OP(dadd),
	OP(isub),
	OP(lsub),
	OP(fsub),
	OP(dsub),
	OP(imul),
	OP(lmul),
	OP(fmul),
	OP(dmul),
	OP(idiv),
	OP(ldiv),
	OP(fdiv),
	OP(ddiv),
	OP(irem),
	OP(lrem),
	OP(frem),
	OP(drem),
	OP(ineg),
	OP(lneg),
	OP(fneg),
	OP(dneg),
	OP(ishl),
	OP(lshl),
	OP(ishr),
	OP(lshr),
	OP(iushr),
	OP(lushr),
	OP(iand),
	OP(land),
	OP(ior),
	OP(lor),
	OP(ixor),
	OP(lxor),
	OP(iinc),
	OP(i2l),
	OP(i2f),
	OP(i2d),
	OP(l2i),
	OP(l2f),
	OP(l2d),
	OP(f2i),
	OP(f2l),
	OP(f2d),
	OP(d2i),
	OP(d2l),
	OP(d2f),
	OP(i2b),
	OP(i2c),
	OP(i2s),
	OP(lcmp),
	OP(fcmpl),
	OP(fcmpg),
	OP(dcmpl),
	OP(dcmpg),
	OP(ifeq),
	OP(ifne),
	OP(iflt),
	OP(ifge),
	OP(ifgt),
	OP(ifle),
	OP(if_icmpeq),
	OP(if_icmpne),
	OP(if_icmplt),
	OP(if_icmpge),
	OP(if_icmpgt),
	OP(if_icmple),
	OP(if_acmpeq),
	OP(if_acmpne),
	OP(goto),
	OP(jsr),
	OP(ret),
	OP(tableswitch),
	OP(lookupswitch),
	OP(ireturn),
	OP(lreturn),
	OP(freturn),
	OP(dreturn),
	OP(areturn),
	OP(return),
	OP(getstatic),
	OP(putstatic),
	OP(getfield),
	OP(putfield),
};

#endif
