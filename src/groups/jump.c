#include <i8080.h>
#include <groups/jump.h>

/* Macros for jumps */
#define JMP(S, A) 	A = S->mem[++S->pc];\
					A |= S->mem[++S->pc] << 8;\
					S->pc = A;

#define JMPC(C, S, A)	if (C) { JMP(S, A); }\
						else { S->pc += 3; }

/* Macros for calls */
#define CALL(S, A)	A = S->mem[++S->pc];\
					A |= S->mem[++S->pc] << 8;\
					S->mem[--S->sp] = ++S->pc >> 8;\
					S->mem[--S->sp] = S->pc;\
					S->pc = A;

#define CALLC(C, S, A) if (C) { CALL(S, A); }\
						else { S->pc += 3; }

/* Macros for returns */
#define RET(S) S->pc = S->mem[S->sp++];\
				S->pc |= S->mem[S->sp++] << 8;

#define RETC(C, S) if (C) { RET(S); }\
					else { S->pc++; }

/* Jump unconditional */
void instr_jmp(I8080_State *s) {
	uint16_t addr;
	DBG("Instruction: jmp\r\n");
	JMP(s, addr);
}

/* Jump on carry */
void instr_jc(I8080_State *s) {
	uint16_t addr;
	DBG("Instruction: jc\r\n");
	JMPC((s->flags & FLG_C), s, addr);
}

/* Jump on no carry */
void instr_jnc(I8080_State *s) {
	uint16_t addr;
	DBG("Instruction: jnc\r\n");
	JMPC(!(s->flags & FLG_C), s, addr);
}

/* Jump on zero */
void instr_jz(I8080_State *s) {
	uint16_t addr;
	DBG("Instruction: jz\r\n");
	JMPC((s->flags & FLG_Z), s, addr);
}

/* Jump on no zero */
void instr_jnz(I8080_State *s) {
	uint16_t addr;
	DBG("Instruction: jnz\r\n");
	JMPC(!(s->flags & FLG_Z), s, addr);
}

/* Jump on positive */
void instr_jp(I8080_State *s) {
	uint16_t addr;
	DBG("Instruction: jp\r\n");
	JMPC(!(s->flags & FLG_S), s, addr);
}

/* Jump on minus */
void instr_jm(I8080_State *s) {
	uint16_t addr;
	DBG("Instruction: jm\r\n");
	JMPC((s->flags & FLG_S), s, addr);
}

/* Jump on parity even */
void instr_jpe(I8080_State *s) {
	uint16_t addr;
	DBG("Instruction: jpe\r\n");
	JMPC((s->flags & FLG_P), s, addr);
}

/* Jump on parity odd */
void instr_jpo(I8080_State *s) {
	uint16_t addr;
	DBG("Instruction: jpo\r\n");
	JMPC(!(s->flags & FLG_P), s, addr);
}

/* H & L to PC */
void instr_pchl(I8080_State *s) {
	DBG("Instruction: pchl\r\n");
	s->pc = s->regs[REG_L];
	s->pc |= s->regs[REG_H] << 8;
}

/* Call unconditional */
void instr_call(I8080_State *s) {
	uint16_t addr;
	DBG("Instruction: call\r\n");
	CALL(s, addr);
}

/* Call on carry */
void instr_cc(I8080_State *s) {
	uint16_t addr;
	DBG("Instruction: cc\r\n");
	CALLC((s->flags & FLG_C), s, addr);
}

/* Call on no carry */
void instr_cnc(I8080_State *s) {
	uint16_t addr;
	DBG("Instruction: cnc\r\n");
	CALLC(!(s->flags & FLG_C), s, addr);
}

/* Call on zero */
void instr_cz(I8080_State *s) {
	uint16_t addr;
	DBG("Instruction: cz\r\n");
	CALLC((s->flags & FLG_Z), s, addr);
}

/* Call on no zero */
void instr_cnz(I8080_State *s) {
	uint16_t addr;
	DBG("Instruction: cnz\r\n");
	CALLC(!(s->flags & FLG_Z), s, addr);
}

/* Call on positive */
void instr_cp(I8080_State *s) {
	uint16_t addr;
	DBG("Instruction: cp\r\n");
	CALLC(!(s->flags & FLG_S), s, addr);
}

/* Call on minus */
void instr_cm(I8080_State *s) {
	uint16_t addr;
	DBG("Instruction: cm\r\n");
	CALLC((s->flags & FLG_S), s, addr);
}

/* Call on parity even */
void instr_cpe(I8080_State *s) {
	uint16_t addr;
	DBG("Instruction: cpe\r\n");
	CALLC((s->flags & FLG_P), s, addr);
}

/* Call on parity odd */
void instr_cpo(I8080_State *s) {
	uint16_t addr;
	DBG("Instruction: cpo\r\n");
	CALLC(!(s->flags & FLG_P), s, addr);
}

/* Return unconditional */
void instr_ret(I8080_State *s) {
	DBG("Instruction: ret\r\n");
	RET(s);
}

/* Return on carry */
void instr_rc(I8080_State *s) {
	DBG("Instruction: rc\r\n");
	RETC((s->flags & FLG_C), s);
}

/* Return on no carry */
void instr_rnc(I8080_State *s) {
	DBG("Instruction: rnc\r\n");
	RETC(!(s->flags & FLG_C), s);
}

/* Return on zero */
void instr_rz(I8080_State *s) {
	DBG("Instruction: rz\r\n");
	RETC((s->flags & FLG_Z), s);
}

/* Return on no zero */
void instr_rnz(I8080_State *s) {
	DBG("Instruction: rnz\r\n");
	RETC(!(s->flags & FLG_Z), s);
}

/* Return on positive */
void instr_rp(I8080_State *s) {
	DBG("Instruction: rp\r\n");
	RETC(!(s->flags & FLG_S), s);
}

/* Return on minus */
void instr_rm(I8080_State *s) {
	DBG("Instruction: rm\r\n");
	RETC((s->flags & FLG_S), s);
}

/* Return on parity even */
void instr_rpe(I8080_State *s) {
	DBG("Instruction: rpe\r\n");
	RETC((s->flags & FLG_P), s);
}

/* Return on parity odd */
void instr_rpo(I8080_State *s) {
	DBG("Instruction: rpo\r\n");
	RETC(!(s->flags & FLG_P), s);
}

/* RESET */
void instr_rst(I8080_State *s) {
	uint16_t addr = s->mem[s->pc++] & 0x38;
	DBG("Instruction: rst\r\n");
	s->mem[--s->sp] = s->pc >> 8;
	s->mem[--s->sp] = s->pc;
	s->pc = addr;
}

