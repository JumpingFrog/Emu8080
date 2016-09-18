#include <i8080.h>
#include <groups/arithmetic.h>

/* Add register to A - Affects: S Z A P C */
void instr_addr(I8080_State *s) {
	uint8_t r = (s->mem[s->pc++] & 0x07);
	uint16_t res = s->regs[REG_A] + s->regs[r];
	DBG("Instruction: addr\r\n");
	/* Carry */
	GEN_CY(res, s);
	/* Aux Carry */
	GEN_AC(s->regs[REG_A], s->regs[r], s);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
}

/* Add memory to A - Affects: S Z A P C */
void instr_addm(I8080_State *s) {
	uint16_t res = s->regs[REG_A] + s->mem[RP_HL(s)];
	DBG("Instruction: addm\r\n");
	/* Carry */
	GEN_CY(res, s)
	/* Aux Carry */
	GEN_AC(s->regs[REG_A], s->mem[RP_HL(s)], s);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
	s->pc++;
}

/* Add immediate to A - Affects: S Z A P C */
void instr_adi(I8080_State *s) {
	uint16_t res = s->regs[REG_A] + s->mem[++s->pc];
	DBG("Instruction: adi\r\n");
	/* Carry */
	GEN_CY(res, s);
	/* Aux Carry */
	GEN_AC(s->regs[REG_A], s->mem[s->pc], s);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
	s->pc++;
}


/* Add memory to A with carry. - Affects: S Z A P C */
void instr_adcm(I8080_State *s) {
	uint16_t res = s->regs[REG_A] + s->mem[RP_HL(s)] + FLAG(s, FLG_C);
	DBG("Instruction: adcm\r\n");
	/* Carry */
	GEN_CY(res, s);
	/* Aux Carry */
	GEN_AC(s->regs[REG_A], s->mem[RP_HL(s)] + FLAG(s, FLG_C), s);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
	s->pc++;
}

/* Add register to A with carry - Affects: S Z A P C */
void instr_adcr(I8080_State *s) {
	uint8_t r = (s->mem[s->pc++] & 0x07);
	uint16_t res = s->regs[REG_A] + s->regs[r] + FLAG(s, FLG_C);
	DBG("Instruction: adcr\r\n");
	/* Carry */
	GEN_CY(res, s);
	/* Aux Carry */
	GEN_AC(s->regs[REG_A], s->regs[r] + FLAG(s, FLG_C), s);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
}

/* Add immediate to A with carry - Affects: S Z A P C */
void instr_aci(I8080_State *s) {
	uint16_t res = s->regs[REG_A] + s->mem[++s->pc] + FLAG(s, FLG_C);
	DBG("Instruction: aci\r\n");
	/* Carry */
	GEN_CY(res, s);
	/* Aux Carry */
	GEN_AC(s->regs[REG_A], s->mem[s->pc] + FLAG(s, FLG_C), s);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
	s->pc++;
}

/* Double add BC to HL - Affects: C */
void instr_dadb(I8080_State *s) {
	uint32_t res = RP_BC(s) + RP_HL(s);
	DBG("Instruction: dadb\r\n");
	/* Carry */
	COND_FLAG(res > 0xFFFF, s, FLG_C);
	/* Update result */
	s->regs[REG_L] = res & 0xFF;
	s->regs[REG_H] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Double add DE to HL - Affects: C */
void instr_dadd(I8080_State *s) {
	uint32_t res = RP_DE(s) + RP_HL(s);
	DBG("Instruction: dadd\r\n");
	/* Carry */
	COND_FLAG(res > 0xFFFF, s, FLG_C);
	/* Update result */
	s->regs[REG_L] = res & 0xFF;
	s->regs[REG_H] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Double add HL to HL - Affects: C */
void instr_dadh(I8080_State *s) {
	uint32_t res = RP_HL(s) << 1;
	DBG("Insturction: dadh\r\n");
	/* Carry */
	COND_FLAG(res > 0xFFFF, s, FLG_C);
	/* Update result */
	s->regs[REG_L] = res & 0xFF;
	s->regs[REG_H] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Double add SP to HL - Affects: C */
void instr_dads(I8080_State *s) {
	uint32_t res = RP_HL(s) + s->sp;
	DBG("Insturction: dads\r\n");
	/* Carry */
	COND_FLAG(res > 0xFFFF, s, FLG_C);
	/* Update result */
	s->regs[REG_L] = res & 0xFF;
	s->regs[REG_H] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Increment register - Affects: S Z A P */
void instr_inrr(I8080_State *s) {
	uint8_t r = (s->mem[s->pc++] & 0x38) >> 3;
	DBG("Instruction: inrr\r\n");
	/* Aux Carry */
	COND_FLAG((s->regs[r] & 0x0F) == 0x0F, s, FLG_A);
	/* Update result */
	s->regs[r] = s->regs[r] + 1;
	gen_pzs(s);
}

/* Increment memory - Affects: S Z A P */
void instr_inrm(I8080_State *s) {
	DBG("Instruction inrm\r\n");
	/* Aux Carry */
	COND_FLAG((s->mem[RP_HL(s)] & 0x0F) == 0x0F, s, FLG_A);
	/* Update result */
	s->mem[RP_HL(s)] = s->mem[RP_HL(s)] + 1;
	s->pc++;
}

/* Decrement register - Affects: S Z A P */
void instr_dcrr(I8080_State *s) {
	uint8_t r = (s->mem[s->pc++] & 0x38) >> 3;
	DBG("Instruction: dcrr\r\n");
	/* Aux Carry - Could do with verifying this on a real chip. */
	COND_FLAG(s->regs[r] & 0x0F, s, FLG_A);
	/* Update result */
	s->regs[r] = s->regs[r] - 1;
	gen_pzs(s);
}

/* Decrement memory - Affects: S Z A P */
void instr_dcrm(I8080_State *s) {
	DBG("Insturction: dcrm\r\n");
	/* Aux Carry - Could do with verifying this on a real chip. */
	COND_FLAG(s->mem[RP_HL(s)] & 0x0F, s, FLG_A);
	/* Update result */
	s->mem[RP_HL(s)] = s->mem[RP_HL(s)] - 1;
	s->pc++;
}

/* Increment register pair - Affects: None */
void instr_inxb(I8080_State *s) {
	uint16_t res = RP_BC(s) + 1;
	DBG("Insturction: inxb\r\n");
	s->regs[REG_C] = res & 0xFF;
	s->regs[REG_B] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Increment register pair - Affects: None */
void instr_inxd(I8080_State *s) {
	uint16_t res = RP_DE(s) + 1;
	DBG("Instruction: inxd\r\n");
	s->regs[REG_E] = res & 0xFF;
	s->regs[REG_D] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Increment register pair - Affects: None */
void instr_inxh(I8080_State *s) {
	uint16_t res = RP_HL(s) + 1;
	DBG("Instruction inxh\r\n");
	s->regs[REG_L] = res & 0xFF;
	s->regs[REG_H] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Increment register SP - Affects: None */
void instr_inxs(I8080_State *s) {
	DBG("Instruction inxs\r\n");
	s->sp++;
	s->pc++;
}

/* Decrement register pair - Affects: None */
void instr_dcxb(I8080_State *s) {
	uint16_t res = RP_BC(s) - 1;
	DBG("Instruction: dcxb\r\n");
	s->regs[REG_C] = res & 0xFF;
	s->regs[REG_B] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Decrement register pair - Affects: None */
void instr_dcxd(I8080_State *s) {
	uint16_t res = RP_DE(s) - 1;
	DBG("Instruction: dcxd\r\n");
	s->regs[REG_E] = res & 0xFF;
	s->regs[REG_D] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Decrement register pair - Affects: None */
void instr_dcxh(I8080_State *s) {
	uint16_t res = RP_HL(s) - 1;
	DBG("Instruction: dcxh\r\n");
	s->regs[REG_L] = res & 0xFF;
	s->regs[REG_H] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Decrement register SP - Affects: None */
void instr_dcxs(I8080_State *s) {
	DBG("Instruction: dcxs\r\n");
	s->sp--;
	s->pc++;
}

/* Subtract register from A - Affects: S Z A P C */
void instr_subr(I8080_State *s) {
	/* Two's complement */
	uint8_t r = ~s->regs[s->mem[s->pc++] & 0x07] + 1;
	uint16_t res = s->regs[REG_A] + r;
	DBG("Instruction: subr\r\n");
	/* Carry */
	COND_FLAG(!(res & 0x100), s, FLG_C);
	/* Aux Carry */
	GEN_AC(s->regs[REG_A], r, s);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
}

/* Subtract memory from A - Affects: S Z A P C */
void instr_subm(I8080_State *s) {
	/* Two's complement */
	uint8_t r = ~s->mem[RP_HL(s)] + 1;
	uint16_t res = s->regs[REG_A] + r;
	DBG("Instruction: subm\r\n");
	/* Carry */
	COND_FLAG(!(res & 0x100), s, FLG_C);
	/* Aux Carry */
	GEN_AC(s->regs[REG_A], r, s);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
	s->pc++;
}

/* Subtract immediate from A - Affects: S Z A P C */
void instr_sui(I8080_State *s) {
	/* Two's complement */
	uint8_t r = ~s->mem[++s->pc] + 1;
	uint16_t res = s->regs[REG_A] + r;
	DBG("Instruction: sui\r\n");
	/* Carry */
	COND_FLAG(!(res & 0x100), s, FLG_C);
	/* Aux Carry */
	GEN_AC(s->regs[REG_A], r, s);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
	s->pc++;
}

/* Subtract register from A with borrow - Affects: S Z A P C */
void instr_sbbr(I8080_State *s) {
	/* Two's complement */
	uint8_t r = s->regs[s->mem[s->pc++] & 0x07] + FLAG(s, FLG_C);
	r = ~r + 1;
	uint16_t res = s->regs[REG_A] + r;
	DBG("Instruction: sbbr\r\n");
	/* Carry */
	COND_FLAG(!(res & 0x100), s, FLG_C);
	/* Aux Carry */
	GEN_AC(s->regs[REG_A], r, s);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
}

/* Subtract memory from A with borrow - Affects: S Z A P C */
void instr_sbbm(I8080_State *s) {
	/* Two's complement */
	uint8_t r = s->mem[RP_HL(s)] + FLAG(s, FLG_C);
	r = ~r + 1;
	uint16_t res = s->regs[REG_A] + r;
	DBG("Instruction: sbbm\r\n");
	/* Carry */
	COND_FLAG(!(res & 0x100), s, FLG_C);
	/* Aux Carry */
	GEN_AC(s->regs[REG_A], r, s);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
	s->pc++;
}

/* Subtract immediate from A with borrow - Affects: S Z A P C */
void instr_sbi(I8080_State *s) {
	/* Two's complement */
	uint8_t r = s->mem[++s->pc] + FLAG(s, FLG_C);
	r = ~r + 1;
	uint16_t res = s->regs[REG_A] + r;
	DBG("Instruction: sbi\r\n");
	/* Carry */
	COND_FLAG(!(res & 0x100), s, FLG_C);
	/* Aux Carry */
	GEN_AC(s->regs[REG_A], r, s);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
	s->pc++;
}

/* Decimal adjust accumulator - Affects: S Z A P C */
void instr_daa(I8080_State *s) {
	uint16_t res;
	DBG("Instruction: daa\r\n");
	if ((s->regs[REG_A] & 0x0F) > 0x09 || FLAG(s, FLG_A)) {
		/* Aux Carry */
		GEN_AC(s->regs[REG_A], 0x06, s);
		s->regs[REG_A] += 0x06;
	}
	// Should this be nested maybe
	if ((s->regs[REG_A] & 0xF0) > 0x90 || FLAG(s, FLG_C)) {
		/* Carry */
		res = s->regs[REG_A] + 0x60;
		GEN_CY(res, s);
		s->regs[REG_A] = res & 0xFF;
	}
	gen_pzs(s);
	s->pc++;
}

