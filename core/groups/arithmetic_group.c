#include "../i8080.h"
#include "arithmetic_group.h"

#define GEN_AC(S, X, Y)  COND_FLAG(((X & 0x0F) + (Y & 0x0F)) > 0x0F, S, FLG_A);

/* Add register to A */
void instr_addr(I8080_State * s) {
	uint8_t r = (s->mem[s->pc++] & 0x07);
	uint16_t res = s->regs[REG_A] + s->regs[r];
	/* Carry */
	COND_FLAG(res > 0xFF, s, FLG_C);
	/* Aux Carry */
	GEN_AC(s, s->regs[REG_A], s->regs[r]);
	/* Update result */
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
}

/* Add register to A with carry */
void instr_adcr(I8080_State * s) {
	uint8_t r = (s->mem[s->pc++] & 0x07);
	uint16_t res = s->regs[REG_A] + s->regs[r];
	/* Carry */
	s->flags |= (res > 0xFF) ? FLG_C : 0;
	/* Aux Carry */
	GEN_AC(s, s->regs[REG_A], s->regs[r]);
	s->regs[REG_A] = res & 0xFF;
	/* PZS Flags */
	gen_pzs(s);
}
