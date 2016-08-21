#include <i8080.h>
#include <groups/jump.h>

/* Macro for conditional jumps */
#define JMPC(C, S) uint16_t addr; if (C) { addr = S->mem[++S->pc];\
											addr |= S->mem[++S->pc] << 8;\
											S->pc = addr; } else { S-> pc += 3; }

/* Jump unconditional */
void instr_jmp(I8080_State * s) {
	uint16_t addr;
	addr = s->mem[++s->pc];
	addr |= s->mem[++s->pc] << 8;
	s->pc = addr;
}

/* Jump on carry */
void instr_jc(I8080_State * s) {
	JMPC((s->flags & FLG_C), s);
}

/* Jump on no carry */
void instr_jnc(I8080_State * s) {
	JMPC(!(s->flags & FLG_C), s);
}

/* Jump on zero */
void instr_jz(I8080_State * s) {
	JMPC((s->flags & FLG_Z), s);
}

/* Jump on no zero */
void instr_jnz(I8080_State * s) {
	JMPC(!(s->flags & FLG_Z), s);
}

/* Jump on positive */
void instr_jp(I8080_State *s) {
	JMPC(!(s->flags & FLG_S), s);
}

/* Jump on minus */
void instr_jm(I8080_State * s) {
	JMPC((s->flags & FLG_S), s);
}

/* Jump on parity even */
void instr_jpe(I8080_State * s) {
	JMPC((s->flags & FLG_P), s);
}

/* Jump on parity odd */
void instr_jpo(I8080_State * s) {
	JMPC(!(s->flags & FLG_P), s);
}

/* H & L to PC */
void instr_pchl(I8080_State * s) {
	s->pc = s->regs[REG_L];
	s->pc |= s->regs[REG_H] << 8;
}
