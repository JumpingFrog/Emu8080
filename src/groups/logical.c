#include <i8080.h>
#include <groups/logical.h>

/* And register with A - Affects: S Z A P C */
void instr_anar(I8080_State *s) {
	uint8_t r = s->regs[s->mem[s->pc] & 0x03];
	DBG(s, "Instruction: anar\r\n");
	/* N.B: The 8080 logical AND instructions set the 
	flag to reflect the logical OR of bit 3 of the values 
	involved in the AND operation. */
	COND_FLAG(s, (r | s->regs[REG_A]) & 0x03, FLG_A);
	COND_FLAG(s, 0, FLG_C);
	WRITE_REG(s, REG_A, s->regs[REG_A] & r);
	GEN_PZS(s, s->regs[REG_A]);
}

/* And memory with A - Affects: S Z A P C */
void instr_anam(I8080_State *s) {
	uint8_t r = s->mem[RP_HL(s)];
	DBG(s, "Instruction: anam\r\n");
	COND_FLAG(s, (r | s->regs[REG_A]) & 0x03, FLG_A);
	COND_FLAG(s, 0, FLG_C);
	WRITE_REG(s, REG_A, s->regs[REG_A] & r);
	GEN_PZS(s, s->regs[REG_A]);
}

/* And immediate with A - Affects: S Z A P C */
void instr_ani(I8080_State *s) {
	uint8_t r = s->mem[s->pc + 1];
	DBG(s, "Instruction: ani\r\n");
	COND_FLAG(s, (r | s->regs[REG_A]) & 0x03, FLG_A);
	COND_FLAG(s, 0, FLG_C);
	WRITE_REG(s, REG_A, s->regs[REG_A] & r);
	GEN_PZS(s, s->regs[REG_A]);
}

/* Compare register with A - Affects: S Z A P C */
void instr_cmpr(I8080_State *s) {
	/* Two's complement */
	uint8_t r = ~s->regs[s->mem[s->pc] & 0x03] + 1;
	uint16_t res = s->regs[REG_A] + r;
	DBG(s, "Instruction: cmpr\r\n");
	/* Carry */
	GEN_CY_SUB(s, res);
	/* Aux Carry */
	GEN_AC(s, s->regs[REG_A], r);
	GEN_PZS(s, res);
}

/* Compare memory with A - Affects: S Z A P C */
void instr_cmpm(I8080_State *s) {
	/* Two's complement */
	uint8_t r = ~s->mem[RP_HL(s)] + 1;
	uint16_t res = s->regs[REG_A] + r;
	DBG(s, "Instruction: cmpm\r\n");
	/* Carry */
	GEN_CY_SUB(S, res);
	/* Aux Carry */
	GEN_AC(s, s->regs[REG_A], r);
	GEN_PZS(s, res);
}

/* Compare immediate with A -  Affects: S Z A P C */
void instr_cpi(I8080_State *s) {
	/* Two's complement */
	uint8_t r = ~s->mem[s->pc + 1] + 1;
	uint16_t res = s->regs[REG_A] + r;
	DBG(s, "Instruction: cpi\r\n");
	s->regs[REG_A] = res;
	/* Carry */
	GEN_CY_SUB(S, res);
	/* Aux Carry */
	GEN_AC(s, s->regs[REG_A], r);
	GEN_PZS(s, res);
}

/* Or register with A - Affects: S Z A P C */
void instr_orar(I8080_State *s) {
	uint8_t r =  s->regs[s->mem[s->pc] & 0x03];
	DBG(s, "Instruction: orar\r\n");
	WRITE_REG(s, REG_A, s->regs[REG_A] | s->regs[r]);
	/* Reset Carry and Aux Carry */
	COND_FLAG(s, 0, FLG_C);
	COND_FLAG(s, 0, FLG_A);
	GEN_PZS(s,s->regs[REG_A]);
}

/* Or memory with A - Affects: S Z A P C */
void instr_oram(I8080_State *s) {
	DBG(s, "Instruction: oram\r\n");
	WRITE_REG(s, REG_A, s->regs[REG_A] | s->mem[RP_HL(s)]);
	/* Reset Carry and Aux Carry */
	COND_FLAG(s, 0, FLG_C);
	COND_FLAG(s, 0, FLG_A);
	GEN_PZS(s, s->regs[REG_A]);
}

/* Or immediate with A - Affects: S Z A P C */
void instr_ori(I8080_State *s) {
	DBG(s, "Instruction: ori\r\n");
	WRITE_REG(s, REG_A, s->regs[REG_A] | s->mem[s->pc + 1]);
	/* Reset Carry and Aux Carry */
	COND_FLAG(s, 0, FLG_C);
	COND_FLAG(s, 0, FLG_A);
	GEN_PZS(s, s->regs[REG_A]);
}

/* Rotate A left through carry - Affects: C */
void instr_ral(I8080_State *s) {
	uint8_t res = s->regs[REG_A] << 1;
	DBG(s, "Instruction: ral\r\n");
	res |= READ_FLAG(s, FLG_C);
	GEN_CY(s, res);
	WRITE_REG(s, REG_A, res);
}

/* Rotate A right through carry - Affects: C */
void instr_rar(I8080_State *s) {
	uint8_t res = s->regs[REG_A] >> 1;
	DBG(s, "Instruction: rar\r\n");
	res |= READ_FLAG(s, FLG_C) << 7;
	COND_FLAG(s, s->regs[REG_A] & 0x1, FLG_C);
	WRITE_REG(s, REG_A, res);
}

/* Rotate A Left - Affects: C */
void instr_rlc(I8080_State *s) {
	uint8_t res = s->regs[REG_A] << 1;
	DBG(s, "Instruction: rlc\r\n");
	res |= (s->regs[REG_A] & 0x80) >> 7;
	GEN_CY(s, res);
	WRITE_REG(s, REG_A, res);
}

/* Rotate A Right - Affects: C */
void instr_rrc(I8080_State *s) {
	uint8_t res = s->regs[REG_A] >> 1;
	DBG(s, "Instruction: rrc\r\n");
	res |= (s->regs[REG_A] & 0x1) << 7;
	COND_FLAG(s, s->regs[REG_A] & 0x1, FLG_C);
	WRITE_REG(s, REG_A, res);
}

/* XOR register with A - Affects: S Z A P C */
void instr_xrar(I8080_State *s) {
	uint8_t r = s->mem[s->pc] & 0x03;
	DBG(s, "Instruction: xrar\r\n");
	WRITE_REG(s, REG_A, s->regs[REG_A] ^ s->regs[r]);
	/* Reset Carry and Aux Carry */
	COND_FLAG(s, 0, FLG_C);
	COND_FLAG(s, 0, FLG_A);
	GEN_PZS(s, s->regs[REG_A]);
}

/* XOR memory with A - Affects: S Z A P C */
void instr_xram(I8080_State *s) {
	DBG(s, "Instruction: xram\r\n");
	WRITE_REG(s, REG_A, s->regs[REG_A] ^ s->mem[RP_HL(s)]);
	/* Reset Carry and Aux Carry */
	COND_FLAG(s, 0, FLG_C);
	COND_FLAG(s, 0, FLG_A);
	GEN_PZS(s, s->regs[REG_A]);
}

/* XOR immediate with A -  Affects: S Z A P C */
void instr_xri(I8080_State *s) {
	DBG(s, "Instruction: xri\r\n");
	WRITE_REG(s, REG_A, s->regs[REG_A] ^ s->mem[s->pc + 1]);
	/* Reset Carry and Aux Carry */
	COND_FLAG(s, 0, FLG_C);
	COND_FLAG(s, 0, FLG_A);
	GEN_PZS(s, s->regs[REG_A]);
}

