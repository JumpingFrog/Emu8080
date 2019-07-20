#include <i8080.h>
#include <groups/arithmetic.h>

/* Add register to A - Affects: S Z A P C */
void instr_addr(I8080_State *s) {
	uint8_t r = READ_MEM(s, s->pc) & 0x07;
	uint16_t res = s->regs[REG_A] + s->regs[r];
	DBG(s, "Instruction: addr\n");
	/* Carry */
	GEN_CY(s, res);
	/* Aux Carry */
	GEN_AC(s, s->regs[REG_A], s->regs[r]);
	/* Update result */
	WRITE_REG(s, REG_A, res);
	/* PZS Flags */
	GEN_PZS(s, s->regs[REG_A]);
}

/* Add memory to A - Affects: S Z A P C */
void instr_addm(I8080_State *s) {
	uint16_t res = s->regs[REG_A] + READ_MEM(s, RP_HL(s));
	DBG(s, "Instruction: addm\n");
	/* Carry */
	GEN_CY(s, res);
	/* Aux Carry */
	GEN_AC(s, s->regs[REG_A], s->mem[RP_HL(s)]);
	/* Update result */
	WRITE_REG(s, REG_A, res);
	/* PZS Flags */
	GEN_PZS(s, s->regs[REG_A]);
}

/* Add immediate to A - Affects: S Z A P C */
void instr_adi(I8080_State *s) {
	uint16_t res = s->regs[REG_A] + READ_IMM8(s);
	DBG(s, "Instruction: adi\n");
	/* Carry */
	GEN_CY(s, res);
	/* Aux Carry */
	GEN_AC(s, s->regs[REG_A], s->mem[s->pc]);
	/* Update result */
	WRITE_REG(s, REG_A, res);
	/* PZS Flags */
	GEN_PZS(s, s->regs[REG_A]);
}


/* Add memory to A with carry. - Affects: S Z A P C */
void instr_adcm(I8080_State *s) {
	uint16_t res = s->regs[REG_A] + s->mem[RP_HL(s)] + READ_FLAG(s, FLG_C);
	DBG(s, "Instruction: adcm\n");
	/* Carry */
	GEN_CY(s, res);
	/* Aux Carry */
	GEN_AC(s, s->regs[REG_A], s->mem[RP_HL(s)] + READ_FLAG(s, FLG_C));
	/* Update result */
	WRITE_REG(s, REG_A, res);
	/* PZS Flags */
	GEN_PZS(s, s->regs[REG_A]);
}

/* Add register to A with carry - Affects: S Z A P C */
void instr_adcr(I8080_State *s) {
	uint8_t r = (s->mem[s->pc] & 0x07);
	uint16_t res = s->regs[REG_A] + s->regs[r] + READ_FLAG(s, FLG_C);
	DBG(s, "Instruction: adcr\n");
	/* Carry */
	GEN_CY(s, res);
	/* Aux Carry */
	GEN_AC(s, s->regs[REG_A], s->regs[r] + READ_FLAG(s, FLG_C));
	/* Update result */
	WRITE_REG(s, REG_A, res);
	/* PZS Flags */
	GEN_PZS(s, s->regs[REG_A]);
}

/* Add immediate to A with carry - Affects: S Z A P C */
void instr_aci(I8080_State *s) {
	uint16_t res = s->regs[REG_A] + s->mem[s->pc + 1] + READ_FLAG(s, FLG_C);
	DBG(s, "Instruction: aci\n");
	/* Carry */
	GEN_CY(s, res);
	/* Aux Carry */
	GEN_AC(s, s->regs[REG_A], s->mem[s->pc] + READ_FLAG(s, FLG_C));
	/* Update result */
	WRITE_REG(s, REG_A, res);
	/* PZS Flags */
	GEN_PZS(s, s->regs[REG_A]);
}

/* Double add BC to HL - Affects: C */
void instr_dadb(I8080_State *s) {
	uint32_t res = RP_BC(s) + RP_HL(s);
	DBG(s, "Instruction: dadb\n");
	/* Carry */
	COND_FLAG(s, res > 0xFFFF, FLG_C);
	/* Update result */
	WRITE_REG(s, REG_L, res);
	WRITE_REG(s, REG_H, res >> 8);
}

/* Double add DE to HL - Affects: C */
void instr_dadd(I8080_State *s) {
	uint32_t res = RP_DE(s) + RP_HL(s);
	DBG(s, "Instruction: dadd\n");
	/* Carry */
	COND_FLAG(s, res > 0xFFFF, FLG_C);
	/* Update result */
	WRITE_REG(s, REG_L, res);
	WRITE_REG(s, REG_H, res >> 8);
}

/* Double add HL to HL - Affects: C */
void instr_dadh(I8080_State *s) {
	uint32_t res = RP_HL(s) << 1;
	DBG(s, "Insturction: dadh\n");
	/* Carry */
	COND_FLAG(s, res > 0xFFFF, FLG_C);
	/* Update result */
	WRITE_REG(s, REG_L, res);
	WRITE_REG(s, REG_H, res >> 8);
}

/* Double add SP to HL - Affects: C */
void instr_dads(I8080_State *s) {
	uint32_t res = RP_HL(s) + s->sp;
	DBG(s, "Insturction: dads\n");
	/* Carry */
	COND_FLAG(s, res > 0xFFFF, FLG_C);
	/* Update result */
	WRITE_REG(s, REG_L, res);
	WRITE_REG(s, REG_H, res >> 8);
}

/* Increment register - Affects: S Z A P */
void instr_inrr(I8080_State *s) {
	uint8_t r = (s->mem[s->pc] & 0x38) >> 3;
	DBG(s, "Instruction: inrr\n");
	/* Aux Carry */
	COND_FLAG(s, (s->regs[r] & 0x0F) == 0x0F, FLG_A);
	/* Update result */
	WRITE_REG(s, r, s->regs[r] + 1);
	GEN_PZS(s, s->regs[REG_A]);
}

/* Increment memory - Affects: S Z A P */
void instr_inrm(I8080_State *s) {
	DBG(s, "Instruction inrm\n");
	/* Aux Carry */
	COND_FLAG(s, (s->mem[RP_HL(s)] & 0x0F) == 0x0F, FLG_A);
	/* Update result */
	WRITE_MEM(s, RP_HL(s), s->mem[RP_HL(s)] + 1);
	GEN_PZS(s, s->regs[REG_A]);
}

/* Decrement register - Affects: S Z A P */
void instr_dcrr(I8080_State *s) {
	uint8_t r = (s->mem[s->pc] & 0x38) >> 3;
	DBG(s, "Instruction: dcrr\n");
	/* Aux Carry - Could do with verifying this on a real chip. */
	COND_FLAG(s, s->regs[r] & 0x0F, FLG_A);
	/* Update result */
	WRITE_REG(s, r, s->regs[r] - 1);
	GEN_PZS(s, s->regs[REG_A]);
}

/* Decrement memory - Affects: S Z A P */
void instr_dcrm(I8080_State *s) {
	DBG(s, "Insturction: dcrm\n");
	/* Aux Carry - Could do with verifying this on a real chip. */
	COND_FLAG(s, s->mem[RP_HL(s)] & 0x0F, FLG_A);
	/* Update result */
	WRITE_MEM(s, RP_HL(s), s->mem[RP_HL(s)] - 1);
	GEN_PZS(s, s->regs[REG_A]);
}

/* Increment register pair - Affects: None */
void instr_inxb(I8080_State *s) {
	uint16_t res = RP_BC(s) + 1;
	DBG(s, "Insturction: inxb\n");
	WRITE_REG(s, REG_C, res);
	WRITE_REG(s, REG_B, res >> 8);
}

/* Increment register pair - Affects: None */
void instr_inxd(I8080_State *s) {
	uint16_t res = RP_DE(s) + 1;
	DBG(s, "Instruction: inxd\n");
	WRITE_REG(s, REG_E, res);
	WRITE_REG(s, REG_D, res >> 8);
}

/* Increment register pair - Affects: None */
void instr_inxh(I8080_State *s) {
	uint16_t res = RP_HL(s) + 1;
	DBG(s, "Instruction inxh\n");
	WRITE_REG(s, REG_L, res);
	WRITE_REG(s, REG_H, res >> 8);
}

/* Increment register SP - Affects: None */
void instr_inxs(I8080_State *s) {
	DBG(s, "Instruction inxs\n");
	WRITE_SP(s, s->sp + 1);
}

/* Decrement register pair - Affects: None */
void instr_dcxb(I8080_State *s) {
	uint16_t res = RP_BC(s) - 1;
	DBG(s, "Instruction: dcxb\n");
	WRITE_REG(s, REG_C, res);
	WRITE_REG(s, REG_B, res >> 8);
}

/* Decrement register pair - Affects: None */
void instr_dcxd(I8080_State *s) {
	uint16_t res = RP_DE(s) - 1;
	DBG(s, "Instruction: dcxd\n");
	WRITE_REG(s, REG_E, res);
	WRITE_REG(s, REG_D, res >> 8);
}

/* Decrement register pair - Affects: None */
void instr_dcxh(I8080_State *s) {
	uint16_t res = RP_HL(s) - 1;
	DBG(s, "Instruction: dcxh\n");
	WRITE_REG(s, REG_L, res);
	WRITE_REG(s, REG_H, res >> 8);
}

/* Decrement register SP - Affects: None */
void instr_dcxs(I8080_State *s) {
	DBG(s, "Instruction: dcxs\n");
	WRITE_SP(s, s->sp - 1);
}

/* Subtract register from A - Affects: S Z A P C */
void instr_subr(I8080_State *s) {
	/* Two's complement */
	uint8_t r = ~s->regs[s->mem[s->pc] & 0x07] + 1;
	uint16_t res = s->regs[REG_A] + r;
	DBG(s, "Instruction: subr\n");
	/* Carry */
	GEN_CY_SUB(s, res);
	/* Aux Carry */
	GEN_AC(s, s->regs[REG_A], r);
	/* Update result */
	WRITE_REG(s, REG_A, res);
	GEN_PZS(s, s->regs[REG_A]);
}

/* Subtract memory from A - Affects: S Z A P C */
void instr_subm(I8080_State *s) {
	/* Two's complement */
	uint8_t r = ~s->mem[RP_HL(s)] + 1;
	uint16_t res = s->regs[REG_A] + r;
	DBG(s, "Instruction: subm\n");
	/* Carry */
	GEN_CY_SUB(s, res);
	/* Aux Carry */
	GEN_AC(s, s->regs[REG_A], r);
	/* Update result */
	WRITE_REG(s, REG_A, res);
	GEN_PZS(s, s->regs[REG_A]);
}

/* Subtract immediate from A - Affects: S Z A P C */
void instr_sui(I8080_State *s) {
	/* Two's complement */
	uint8_t r = ~s->mem[s->pc + 1] + 1;
	uint16_t res = s->regs[REG_A] + r;
	DBG(s, "Instruction: sui\n");
	/* Carry */
	GEN_CY_SUB(s, res);
	/* Aux Carry */
	GEN_AC(s, s->regs[REG_A], r);
	/* Update result */
	WRITE_REG(s, REG_A, res);
	GEN_PZS(s, s->regs[REG_A]);
}

/* Subtract register from A with borrow - Affects: S Z A P C */
void instr_sbbr(I8080_State *s) {
	/* Two's complement */
	uint8_t r = s->regs[s->mem[s->pc] & 0x07] + READ_FLAG(s, FLG_C);
	r = ~r + 1;
	uint16_t res = s->regs[REG_A] + r;
	DBG(s, "Instruction: sbbr\n");
	/* Carry */
	GEN_CY_SUB(s, res);
	/* Aux Carry */
	GEN_AC(s, s->regs[REG_A], r);
	/* Update result */
	WRITE_REG(s, REG_A, res);
	GEN_PZS(s, s->regs[REG_A]);
}

/* Subtract memory from A with borrow - Affects: S Z A P C */
void instr_sbbm(I8080_State *s) {
	/* Two's complement */
	uint8_t r = s->mem[RP_HL(s)] + READ_FLAG(s, FLG_C);
	r = ~r + 1;
	uint16_t res = s->regs[REG_A] + r;
	DBG(s, "Instruction: sbbm\n");
	/* Carry */
	GEN_CY_SUB(s, res);
	/* Aux Carry */
	GEN_AC(s, s->regs[REG_A], r);
	/* Update result */
	WRITE_REG(s, REG_A, res);
	GEN_PZS(s, s->regs[REG_A]);
}

/* Subtract immediate from A with borrow - Affects: S Z A P C */
void instr_sbi(I8080_State *s) {
	/* Two's complement */
	uint8_t r = s->mem[s->pc + 1] + READ_FLAG(s, FLG_C);
	r = ~r + 1;
	uint16_t res = s->regs[REG_A] + r;
	DBG(s, "Instruction: sbi\n");
	/* Carry */
	GEN_CY_SUB(s, res);
	/* Aux Carry */
	GEN_AC(s, s->regs[REG_A], r);
	/* Update result */
	WRITE_REG(s, REG_A, res);
	/* PZS Flags */
	GEN_PZS(s, s->regs[REG_A]);
}

/* Decimal adjust accumulator - Affects: S Z A P C */
void instr_daa(I8080_State *s) {
	uint16_t res;
	DBG(s, "Instruction: daa\n");
	if ((s->regs[REG_A] & 0x0F) > 0x09 || READ_FLAG(s, FLG_A)) {
		/* Aux Carry */
		GEN_AC(s, s->regs[REG_A], 0x06);
		WRITE_REG(s, REG_A, s->regs[REG_A] + 0x06);
	}
	if ((s->regs[REG_A] & 0xF0) > 0x90 || READ_FLAG(s, FLG_C)) {
		/* Carry */
		res = s->regs[REG_A] + 0x60;
		GEN_CY(s, res);
		WRITE_REG(s, REG_A, res);
	}
	GEN_PZS(s, s->regs[REG_A]);
}

