#include <i8080.h>
#include <groups/stack.h>

#define PUSH(S, H, L)	WRITE_MEM(S, (S)->sp - 1, (S)->regs[H]);\
						WRITE_MEM(S, (S)->sp - 2, (S)->regs[L]);\
						WRITE_SP(S, (S)->sp - 2)

#define POP(S, H, L)	WRITE_REG(S, L, (S)->mem[(S)->sp]);\
						WRITE_REG(S, H, (S)->mem[(S)->sp + 1]);\
						WRITE_SP(S, (S)->sp + 2)

/* Push B & C */
void instr_pushb(I8080_State *s) {
	DBG(s, "Instruction: pushb\r\n");
	PUSH(s, REG_B, REG_C);
}

/* Push D & E */
void instr_pushd(I8080_State *s) {
	DBG(s, "Instruction: pushd\r\n");
	PUSH(s, REG_D, REG_E);
}

/* Push H & L */
void instr_pushh(I8080_State *s) {
	DBG(s, "Instruction: pushh\r\n");
	PUSH(s, REG_H, REG_L);
}

/* Push PSW */
void instr_pushp(I8080_State *s) {
	DBG(s, "Instruction: pushp\r\n");
	/* N.B. Not macro compatiable. */
	WRITE_MEM(s, s->sp - 1, s->regs[REG_A]);\
	WRITE_MEM(s, s->sp - 2, s->flags);\
	WRITE_SP(s, s->sp - 2);
}

/* Pop B & C */
void instr_popb(I8080_State *s) {
	DBG(s, "Instruction: popb\r\n");
	POP(s, REG_B, REG_C);
}

/* Pop D & E */
void instr_popd(I8080_State *s) {
	DBG(s, "Instruction: popd\r\n");
	POP(s, REG_D, REG_E);
}

/* Pop H & L */
void instr_poph(I8080_State *s) {
	DBG(s, "Instruction: poph\r\n");
	POP(s, REG_H, REG_L);
}

/* Pop PSW */
void instr_popp(I8080_State *s) {
	DBG(s, "Instruction: popp\r\n");
	/* N.B. Not macro compatiable. */
	WRITE_FLAGS(s, s->mem[s->sp]);
	WRITE_REG(s, REG_A, s->mem[s->sp + 1]);
	WRITE_SP(s, s->sp + 2);
}

/* Exchange top of stack with H & L */
void instr_xthl(I8080_State *s) {
	uint8_t temp = s->regs[REG_L];
	DBG(s, "Instruction: xthl\r\n");
	WRITE_REG(s, REG_L, s->mem[s->sp]);
	WRITE_MEM(s, s->sp, temp);
	temp = s->regs[REG_H];
	WRITE_REG(s, REG_H, s->mem[s->sp + 1]);
	WRITE_MEM(s, s->sp + 1, temp);
}

/* H & L to SP */
void instr_sphl(I8080_State *s) {
	DBG(s, "Instruction: sphl\r\n");
	WRITE_SP(s, RP_HL(s));
}

