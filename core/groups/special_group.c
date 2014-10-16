#include "../i8080.h"
#include "special_group.h"

/*RESET*/
void instr_rst(I8080_State * s) {
	uint8_t v = (s->mem[s->pc++] & 0x38);
	s->mem[--s->sp] = (s->pc & 0xFF00) >> 8;
	s->mem[--s->sp] = (s->pc & 0x00FF) >> 8; 
	s->pc = 0x0000 | v;
}

/* I/O */
void instr_in(I8080_State * s) {
	uint8_t p = s->mem[++s->pc];
	s->regs[REG_A] = (*s->devices[p]->in)(s);
}

void instr_out(I8080_State * s) {
	uint8_t p = s->mem[++s->pc];
	(*s->devices[p]->out)(s);
}

/* Interrupt just NOPs */
void instr_ei(I8080_State * s) {
	s->pc++;
}

void instr_di(I8080_State * s) {
	s->pc++;
}

void instr_nop(I8080_State * s) {
	s->pc++;
}

void instr_hlt(I8080_State * s) {
	s->hlt = 1;
}