#include "../i8080.h"
#include "arithmetic_group.h"

/* Add register to A */
void instr_addr(I8080_State * s) {
	uint8_t r = (s->mem[s->pc++] & 0x07);
	uint16_t res = s->regs[REG_A] + s->regs[r];
	/* Carry */
	s->flags |= (res > 0xFF) ? FLG_C : 0;
	/* Aux Carry */
	s->flags |= (((s->regs[REG_A] & 0x0F) + (s->regs[r] & 0x0F)) > 0x0F) ?
								FLG_A : 0;
	s->regs[REG_A] = res & 0xFF;
	gen_pzs(s);
}

/* Add register to A with carry */
void instr_adcr(I8080_State * s) {
	uint8_t r = (s->mem[s->pc++] & 0x07);
	uint16_t res = s->regs[REG_A] + s->regs[r];
}
