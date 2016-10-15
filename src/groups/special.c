#include <i8080.h>
#include <groups/special.h>

/* Complement Accumulator */
void instr_cma(I8080_State *s) {
	DBG(s, "Instruction: cma\r\n");
	s->regs[REG_A] = ~(s->regs[REG_A]);
	s->pc++;
}

/* Complement Carry */
void instr_cmc(I8080_State *s) {
	DBG(s, "Instruction: cmc\r\n");
	s->flags ^= FLG_C;
	s->pc++;
}

/* Set Carry */
void instr_stc(I8080_State *s) {
	DBG(s, "Instruction: stc\r\n");
	s->flags |= FLG_C;
	s->pc++;
}

/* I/O */
void instr_in(I8080_State *s) {
	uint8_t p = s->mem[++s->pc];
	DBG(s, "Instruction: in\r\n");
	if (s->devices[p]) {
		s->regs[REG_A] = (*s->devices[p]->in)(s);
	}
	s->pc++;
}

void instr_out(I8080_State *s) {
	uint8_t p = s->mem[++s->pc];
	DBG(s, "Instruction: out\r\n");
	if (s->devices[p]) {
		(*s->devices[p]->out)(s);
	}
	s->pc++;
}

/* Interrupt just NOPs */
void instr_ei(I8080_State *s) {
	DBG(s, "Instruction: ei\r\n");
	s->pc++;
}

void instr_di(I8080_State *s) {
	DBG(s, "Instruction: di\r\n");
	s->pc++;
}

/* No operation */
void instr_nop(I8080_State *s) {
	DBG(s, "Instruction: nop\r\n");
	s->pc++;
}

/* Halt */
void instr_hlt(I8080_State *s) {
	DBG(s, "Instruction: hlt\r\n");
	s->hlt = 1;
}

