#include "../i8080.h"
#include "data_group.h"

/* Move Immediate Register */
void instr_mvir(I8080_State * s) {
	uint8_t r = s->mem[s->pc++] & 0x03;
	s->regs[r] = s->mem[s->pc++];
}

/* Move Immediate Memory (HL) */
void instr_mvim(I8080_State * s) {

}