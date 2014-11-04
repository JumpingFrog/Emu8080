#include "../i8080.h"
#include "jump_group.h"

void instr_jmp(I8080_State * s) {
	uint16_t addr;
	addr = s->mem[++s->pc];
	addr |= s->mem[++s->pc] << 8;
	s->pc = addr;
}
