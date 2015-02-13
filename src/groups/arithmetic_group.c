#include "../i8080.h"
#include "arithmetic_group.h"

/* Add register to A */
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

/* Add register to A with carry */
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

/* Add memory to A */
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
}

/* Add memory to A with carry. */
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
}
