#include <i8080.h>
#include <groups/jump.h>

/* Macros for jumps */
#define JMP(S) WRITE_PC(S, READ_IMM16(S))

#define JMPC(S, C)	if (C) { JMP(S); }

/* Macros for calls */
/* N.B. PC + 3 to account for IMM 16 */
#define CALL(S)	WRITE_MEM(S, (S)->sp - 1, ((S)->pc + 3) >> 8);\
				WRITE_MEM(S, (S)->sp - 2, (S)->pc + 3);\
				WRITE_SP(S, (S)->sp - 2);\
				JMP(S)

#define CALLC(S, C) if (C) { CALL(S); }

/* Macros for returns */
#define RET(S)	WRITE_PC(S, (READ_MEM(S, (S)->sp + 1) << 8) | READ_MEM(S, (S)->sp));\
				WRITE_SP(S, (S)->sp + 2)

#define RETC(S, C) if (C) { RET(S); }

/* Jump unconditional */
void instr_jmp(I8080State *s) {
	DBG(s, "Instruction: jmp\n");
	JMP(s);
}

/* Jump on carry */
void instr_jc(I8080State *s) {
	DBG(s, "Instruction: jc\n");
	JMPC(s, READ_FLAG(s, FLG_C));
}

/* Jump on no carry */
void instr_jnc(I8080State *s) {
	DBG(s, "Instruction: jnc\n");
	JMPC(s, !READ_FLAG(s, FLG_C));
}

/* Jump on zero */
void instr_jz(I8080State *s) {
	DBG(s, "Instruction: jz\n");
	JMPC(s, READ_FLAG(s, FLG_Z));
}

/* Jump on no zero */
void instr_jnz(I8080State *s) {
	DBG(s, "Instruction: jnz\n");
	JMPC(s, !READ_FLAG(s, FLG_Z));
}

/* Jump on positive */
void instr_jp(I8080State *s) {
	DBG(s, "Instruction: jp\n");
	JMPC(s, !READ_FLAG(s, FLG_S));
}

/* Jump on minus */
void instr_jm(I8080State *s) {
	DBG(s, "Instruction: jm\n");
	JMPC(s, READ_FLAG(s, FLG_S));
}

/* Jump on parity even */
void instr_jpe(I8080State *s) {
	DBG(s, "Instruction: jpe\n");
	JMPC(s, READ_FLAG(s, FLG_P));
}

/* Jump on parity odd */
void instr_jpo(I8080State *s) {
	DBG(s, "Instruction: jpo\n");
	JMPC(s, !READ_FLAG(s, FLG_C));
}

/* H & L to PC */
void instr_pchl(I8080State *s) {
	DBG(s, "Instruction: pchl\n");
	WRITE_PC(s, RP_HL(s));
}

/* Call unconditional */
void instr_call(I8080State *s) {
	DBG(s, "Instruction: call\n");
	CALL(s);
}

/* Call on carry */
void instr_cc(I8080State *s) {
	DBG(s, "Instruction: cc\n");
	CALLC(s, READ_FLAG(s, FLG_C));
}

/* Call on no carry */
void instr_cnc(I8080State *s) {
	DBG(s, "Instruction: cnc\n");
	CALLC(s, !READ_FLAG(s, FLG_C));
}

/* Call on zero */
void instr_cz(I8080State *s) {
	DBG(s, "Instruction: cz\n");
	CALLC(s, READ_FLAG(s, FLG_Z));
}

/* Call on no zero */
void instr_cnz(I8080State *s) {
	DBG(s, "Instruction: cnz\n");
	CALLC(s, !READ_FLAG(s, FLG_Z));
}

/* Call on positive */
void instr_cp(I8080State *s) {
	DBG(s, "Instruction: cp\n");
	CALLC(s, !READ_FLAG(s, FLG_S));
}

/* Call on minus */
void instr_cm(I8080State *s) {
	DBG(s, "Instruction: cm\n");
	CALLC(s, READ_FLAG(s, FLG_S));
}

/* Call on parity even */
void instr_cpe(I8080State *s) {
	DBG(s, "Instruction: cpe\n");
	CALLC(s, READ_FLAG(s, FLG_P));
}

/* Call on parity odd */
void instr_cpo(I8080State *s) {
	DBG(s, "Instruction: cpo\n");
	CALLC(s, !READ_FLAG(s, FLG_P));
}

/* Return unconditional */
void instr_ret(I8080State *s) {
	DBG(s, "Instruction: ret\n");
	RET(s);
}

/* Return on carry */
void instr_rc(I8080State *s) {
	DBG(s, "Instruction: rc\n");
	RETC(s, READ_FLAG(s, FLG_C));
}

/* Return on no carry */
void instr_rnc(I8080State *s) {
	DBG(s, "Instruction: rnc\n");
	RETC(s, !READ_FLAG(s, FLG_C));
}

/* Return on zero */
void instr_rz(I8080State *s) {
	DBG(s, "Instruction: rz\n");
	RETC(s, READ_FLAG(s, FLG_Z));
}

/* Return on no zero */
void instr_rnz(I8080State *s) {
	DBG(s, "Instruction: rnz\n");
	RETC(s, !READ_FLAG(s, FLG_Z));
}

/* Return on positive */
void instr_rp(I8080State *s) {
	DBG(s, "Instruction: rp\n");
	RETC(s, !READ_FLAG(s, FLG_S));
}

/* Return on minus */
void instr_rm(I8080State *s) {
	DBG(s, "Instruction: rm\n");
	RETC(s, READ_FLAG(s, FLG_S));
}

/* Return on parity even */
void instr_rpe(I8080State *s) {
	DBG(s, "Instruction: rpe\n");
	RETC(s, READ_FLAG(s, FLG_P));
}

/* Return on parity odd */
void instr_rpo(I8080State *s) {
	DBG(s, "Instruction: rpo\n");
	RETC(s, !READ_FLAG(s, FLG_P));
}

/* RESET */
void instr_rst(I8080State *s) {
	uint8_t vector = s->opcode & 0x38;
	DBG(s, "Instruction: rst\n");
	WRITE_MEM(s, s->sp - 1, (s->pc + 1) >> 8);
	WRITE_MEM(s, s->sp - 2, s->pc + 1);
	WRITE_SP(s, s->sp - 2);
	WRITE_PC(s, vector);
}

