#include <i8080.h>
#include <groups/special.h>

/* Complement Accumulator - Affects: None */
void instr_cma(I8080State *s) {
	DBG(s, "Instruction: cma\n");
	WRITE_REG(s, REG_A, ~(s->regs[REG_A]));
}

/* Complement Carry - Affects: C */
void instr_cmc(I8080State *s) {
	DBG(s, "Instruction: cmc\n");
	COND_FLAG(s, !READ_FLAG(s, FLG_C), FLG_C);
}

/* Set Carry - Affects: C */
void instr_stc(I8080State *s) {
	DBG(s, "Instruction: stc\n");
	COND_FLAG(s, 1, FLG_C);
}

/* I/O */
void instr_in(I8080State *s) {
	uint8_t p = s->mem[s->pc + 1];
	DBG(s, "Instruction: in\n");
	if (s->dev_map[p]) {
		WRITE_REG(s, REG_A, (*s->dev_map[p]->in)(s));
	}
}

void instr_out(I8080State *s) {
	uint8_t p = s->mem[s->pc + 1];
	DBG(s, "Instruction: out\n");
	if (s->dev_map[p]) {
		(*s->dev_map[p]->out)(s);
	}
}

/* Enable interrupts */
void instr_ei(I8080State *s) {
	DBG(s, "Instruction: ei\n");
	s->int_ff = 1;
}

/* Disable interrupts */
void instr_di(I8080State *s) {
	DBG(s, "Instruction: di\n");
	s->int_ff = 0;
}

/* No operation */
void instr_nop(I8080State *s) {
	DBG(s, "Instruction: nop\n");
}

/* Halt */
void instr_hlt(I8080State *s) {
	DBG(s, "Instruction: hlt\n");
	s->hlt = 1;
}

