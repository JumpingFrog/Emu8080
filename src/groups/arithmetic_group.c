#include "../i8080.h"
#include "arithmetic_group.h"

/* Add register to A - Affects: S Z A P C */
void instr_addr(I8080_State * s) {
	uint8_t r = (s->mem[s->pc++] & 0x07);
	uint16_t res = s->regs[REG_A] + s->regs[r];
	/* Carry */
	COND_FLAG(res > 0xFF, s, FLG_C);
	/* Aux Carry */
	COND_FLAG(((s->regs[REG_A] & 0x0F) + (s->regs[r] & 0x0F)) > 0x0F, s, FLG_A);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
}

/* Add register to A with carry - Affects: S Z A P C */
void instr_adcr(I8080_State * s) {
	uint8_t r = (s->mem[s->pc++] & 0x07);
	uint16_t res = s->regs[REG_A] + s->regs[r] + FLAG(s, FLG_C);
	/* Carry */
	COND_FLAG(res > 0xFF, s, FLG_C);
	/* Aux Carry */
	COND_FLAG(((s->regs[REG_A] & 0x0F) + (s->regs[r] & 0x0F) + FLAG(s, FLG_C)) > 0x0F, s, FLG_A);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
}

/* Add memory to A - Affects: S Z A P C */
void instr_addm(I8080_State * s) {
	uint16_t res = s->regs[REG_A] + s->mem[MEM(s)];
	/* Carry */
	COND_FLAG(res > 0xFF, s, FLG_C);
	/* Aux Carry */
	COND_FLAG(((s->regs[REG_A] & 0x0F) + (s->mem[MEM(s)] & 0x0F)) > 0x0F, s, FLG_A);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
	s->pc++;
}

/* Add memory to A with carry. - Affects: S Z A P C */
void instr_adcm(I8080_State * s) {
	uint16_t res = s->regs[REG_A] + s->mem[MEM(s)] + FLAG(s, FLG_C);
	/* Carry */
	COND_FLAG(res > 0xFF, s, FLG_C);
	/* Aux Carry */
	COND_FLAG(((s->regs[REG_A] & 0x0F) + (s->mem[MEM(s)] & 0x0F) + FLAG(s, FLG_C)) > 0x0F, s, FLG_A);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
	s->pc++;
}

/* Add immediate to A - Affects: S Z A P C */
void instr_adi(I8080_State * s) {
	uint16_t res = s->regs[REG_A] + s->mem[++s->pc];
	/* Carry */
	COND_FLAG(res > 0xFF, s, FLG_C);
	/* Aux Carry */
	COND_FLAG(((s->regs[REG_A] & 0x0F) + (s->mem[s->pc] & 0x0F)) > 0x0F, s, FLG_A);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
	s->pc++;
}

/* Add immediate to A with carry - Affects: S Z A P C */
void instr_aci(I8080_State * s) {
	uint16_t res = s->regs[REG_A] + s->mem[++s->pc] + FLAG(s, FLG_C);
	/* Carry */
	COND_FLAG(res > 0xFF, s, FLG_C);
	/* Aux Carry */
	COND_FLAG(((s->regs[REG_A] & 0x0F) + (s->mem[s->pc] & 0x0F) + FLAG(s, FLG_C)) > 0x0F, s, FLG_A);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
	s->pc++;
}

/* Double add BC to HL - Affects: C */
void instr_dadb(I8080_State * s) {
	uint32_t res = RP_BC(s) + RP_HL(s);
	/* Carry */
	COND_FLAG(res > 0xFFFF, s, FLG_C);
	/* Update result */
	s->regs[REG_L] = res & 0xFF;
	s->regs[REG_H] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Double add DE to HL - Affects: C */
void instr_dadd(I8080_State * s) {
	uint32_t res = RP_DE(s) + RP_HL(s);
	/* Carry */
	COND_FLAG(res > 0xFFFF, s, FLG_C);
	/* Update result */
	s->regs[REG_L] = res & 0xFF;
	s->regs[REG_H] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Double add HL to HL - Affects: C */
void instr_dadh(I8080_State * s) {
	uint32_t res = RP_HL(s) << 1;
	/* Carry */
	COND_FLAG(res > 0xFFFF, s, FLG_C);
	/* Update result */
	s->regs[REG_L] = res & 0xFF;
	s->regs[REG_H] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Double add SP to HL - Affects: C */
void instr_dads(I8080_State * s) {
	uint32_t res = RP_HL(s) + s->sp;
	/* Carry */
	COND_FLAG(res > 0xFFFF, s, FLG_C);
	/* Update result */
	s->regs[REG_L] = res & 0xFF;
	s->regs[REG_H] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Increment register - Affects: S Z A P */
void instr_inrr(I8080_State * s) {
	uint8_t r = (s->mem[s->pc++] & 0x38) >> 3;
	/* Aux Carry */
	COND_FLAG((s->regs[r] & 0x0F) == 0x0F, s, FLG_A);
	/* Update result */
	s->regs[r] = s->regs[r] + 1;
	gen_pzs(s);
}

/* Increment memory - Affects: S Z A P */
void instr_inrm(I8080_State * s) {
	/* Aux Carry */
	COND_FLAG((s->mem[RP_HL(s)] & 0x0F) == 0x0F, s, FLG_A);
	/* Update result */
	s->mem[RP_HL(s)] = s->mem[RP_HL(s)] + 1;
	s->pc++;
}

/* Decrement register - Affects: S Z A P  */
void instr_dcrr(I8080_State * s) {
	uint8_t r = (s->mem[s->pc++] & 0x38) >> 3;
	/* Aux Carry - Could do with verifying this on a real chip. */
	COND_FLAG(s->regs[r] & 0x0F, s, FLG_A);
	/* Update result */
	s->regs[r] = s->regs[r] - 1;
	gen_pzs(s);
}

/* Decrement memory - Affects: S Z A P */
void instr_dcrm(I8080_State * s) {
	/* Aux Carry - Could do with verifying this on a real chip. */
	COND_FLAG(s->mem[RP_HL(s)] & 0x0F, s, FLG_A);
	/* Update result */
	s->mem[RP_HL(s)] = s->mem[RP_HL(s)] - 1;
	s->pc++;
}

/* Increment register pair - Affects: None */
void instr_inxb(I8080_State * s) {
	uint16_t res = RP_BC(s) + 1;
	s->regs[REG_C] = res & 0xFF;
	s->regs[REG_B] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Increment register pair - Affects: None */
void instr_inxd(I8080_State * s) {
	uint16_t res = RP_DE(s) + 1;
	s->regs[REG_E] = res & 0xFF;
	s->regs[REG_D] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Increment register pair - Affects: None */
void instr_inxh(I8080_State * s) {
	uint16_t res = RP_HL(s) + 1;
	s->regs[REG_L] = res & 0xFF;
	s->regs[REG_H] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Decrement register pair - Affects: None */
void instr_dcxb(I8080_State * s) {
	uint16_t res = RP_BC(s) - 1;
	s->regs[REG_C] = res & 0xFF;
	s->regs[REG_B] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Decrement register pair - Affects: None */
void instr_dcxd(I8080_State * s) {
	uint16_t res = RP_DE(s) - 1;
	s->regs[REG_E] = res & 0xFF;
	s->regs[REG_D] = (res >> 8) & 0xFF;
	s->pc++;
}

/* Decrement register pair - Affects: None */
void instr_dcxh(I8080_State * s) {
	uint16_t res = RP_HL(s) - 1;
	s->regs[REG_L] = res & 0xFF;
	s->regs[REG_H] = (res >> 8) & 0xFF;
	s->pc++;
}
