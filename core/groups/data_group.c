#include "../i8080.h"
#include "data_group.h"

/* Move Immediate Register */
void instr_mvir(I8080_State * s) {
	uint8_t r = (s->mem[s->pc++] & 0x38) >> 3;
	s->regs[r] = s->mem[s->pc++];
}

/* Move Immediate Memory (HL) */
void instr_mvim(I8080_State * s) {
	s->pc++;
	s->mem[(s->regs[REG_H] << 8) | s->regs[REG_L]] = s->mem[s->pc++];
}