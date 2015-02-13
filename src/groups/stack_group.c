#include "../i8080.h"
#include "stack_group.h"

#define PUSH(H, L, S) S->mem[--S->sp] = H; S->mem[--S->sp] = L;

#define POP(H, L, S) L = S->mem[S->sp++]; H = S->mem[S->sp++];

/* Push B & C */
void instr_pushb(I8080_State * s) {
	PUSH(s->regs[REG_B], s->regs[REG_C], s);
	s->pc++;
}

/* Push D & E */
void instr_pushd(I8080_State * s) {
	PUSH(s->regs[REG_D], s->regs[REG_E], s);
	s->pc++;
}

/* Push H & L */
void instr_pushh(I8080_State * s) {
	PUSH(s->regs[REG_H], s->regs[REG_L], s);
	s->pc++;
}

/* Push PSW */
void instr_pushp(I8080_State * s) {
	PUSH(s->regs[REG_A], s->flags, s);
	s->pc++;
}

/* Pop B & C */
void instr_popb(I8080_State * s) {
	POP(s->regs[REG_B], s->regs[REG_C], s);
	s->pc++;
}

/* Pop D & E */
void instr_popd(I8080_State * s) {
	POP(s->regs[REG_D], s->regs[REG_E], s);
	s->pc++;
}

/* Pop H & L */
void instr_poph(I8080_State * s) {
	POP(s->regs[REG_H], s->regs[REG_L], s);
	s->pc++;
}

/* Pop PSW */
void instr_popp(I8080_State * s) {
	POP(s->regs[REG_A], s->flags, s);
	/* Maintain constant 2 */
	s->flags |= 0x02;
	/* Maintain 0 constants */
	s->flags &= 0xd7;
	s->pc++;
}

/* Exchange top of stack with H & L */
void instr_xthl(I8080_State * s) {
	uint8_t temp = s->regs[REG_L];
	s->regs[REG_L] = s->mem[s->sp];
	s->mem[s->sp] = temp;
	temp = s->regs[REG_H];
	s->regs[REG_H] = s->mem[s->sp + 1];
	s->mem[s->sp + 1] = temp;
	s->pc++;
}

/* H & L to SP */
void instr_sphl(I8080_State * s) {
	s->sp = s->regs[REG_L];
	s->sp |= s->regs[REG_H] << 0x08;
	s->pc++;
}

/* Load immediate SP */
void instr_lxisp(I8080_State * s) {
	s->pc++;
	s->sp = s->mem[s->pc++];
	s->sp |= s->mem[s->pc++] << 0x08;
}

/* Increment SP */
void instr_inxsp(I8080_State * s) {
	s->sp++;
	s->pc++;
}

/* Decrement SP */
void instr_dcxsp(I8080_State * s) {
	s->sp--;
	s->pc++;
}
