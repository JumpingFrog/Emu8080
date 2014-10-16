#include "../i8080.h"
#include "stack_group.h"

#define PUSH(H, L, S) S->mem[--S->sp] = H; S->mem[--S->sp] = L;

#define POP(H, L, S) L = S->mem[S->sp++]; H = S->mem[S-sp++];

void instr_pushb(I8080_State * s) {
	PUSH(s->regs[REG_B], s->regs[REG_C], s);
	s->pc++;
}

void instr_pushd(I8080_State * s) {
	PUSH(REG_D, REG_E, s);
	s->pc++;
}

void instr_pushh(I8080_State * s) {
	PUSH(REG_H, REG_L, s);
	s->pc++;
}

/* Push PSW */
void instr_pushp(I8080_State * s) {
	PUSH(s->pc, s->flags, s);
	s->pc++;
}