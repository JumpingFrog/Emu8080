#include <i8080.h>
#include <groups/logical.h>

/* And register with A - Affects: S Z A P C */
void instr_anar(I8080_State *s) {
	uint8_t r = s->regs[s->mem[s->pc++] & 0x03];
	DBG(s, "Instruction: anar\r\n");
	/* N.B: The 8080 logical AND instructions set the 
	flag to reflect the logical OR of bit 3 of the values 
	involved in the AND operation. */
	COND_FLAG((r | s->regs[REG_A]) & 0x03, s, FLG_A);
	COND_FLAG(0, s, FLG_C);
	s->regs[REG_A] &= r;
	gen_pzs(s);
}

/* And memory with A - Affects: S Z A P C */
void instr_anam(I8080_State *s) {
	uint8_t r = s->mem[RP_HL(s)];
	DBG(s, "Instruction: anam\r\n");
	COND_FLAG((r | s->regs[REG_A]) & 0x03, s, FLG_A);
	COND_FLAG(0, s, FLG_C);
	s->regs[REG_A] &= r;
	gen_pzs(s);
	s->pc++;
}

/* And immediate with A - Affects: S Z A P C */
void instr_ani(I8080_State *s) {
	uint8_t r = s->mem[++s->pc];
	DBG(s, "Instruction: ani\r\n");
	COND_FLAG((r | s->regs[REG_A]) & 0x03, s, FLG_A);
	COND_FLAG(0, s, FLG_C);
	s->regs[REG_A] &= r;
	gen_pzs(s);
	s->pc++;
}

/* Compare register with A - Affects: S Z A P C */
void instr_cmpr(I8080_State *s) {
	/* Two's complement */
	uint8_t r = ~s->regs[s->mem[s->pc++] & 0x03] + 1;
	uint16_t res = s->regs[REG_A] + r;
	DBG(s, "Instruction: cmpr\r\n");
	/* Carry */
	COND_FLAG(!(res & 0x100), s, FLG_C);
	/* Aux Carry */
	GEN_AC(s->regs[REG_A], r, s);
	gen_pzs(s);
}

/* Compare memory with A - Affects: S Z A P C */
void instr_cmpm(I8080_State *s) {
	/* Two's complement */
	uint8_t r = ~s->mem[RP_HL(s)] + 1;
	uint16_t res = s->regs[REG_A] + r;
	DBG(s, "Instruction: cmpm\r\n");
	/* Carry */
	COND_FLAG(!(res & 0x100), s, FLG_C);
	/* Aux Carry */
	GEN_AC(s->regs[REG_A], r, s);
	gen_pzs(s);
	s->pc++;
}

/* Compare immediate with A -  Affects: S Z A P C */
void instr_cpi(I8080_State *s) {
	/* Two's complement */
	uint8_t r = ~s->mem[++s->pc] + 1;
	uint16_t res = s->regs[REG_A] + r;
	DBG(s, "Instruction: cpi\r\n");
	/* Carry */
	COND_FLAG(!(res & 0x100), s, FLG_C);
	/* Aux Carry */
	GEN_AC(s->regs[REG_A], r, s);
	gen_pzs(s);
	s->pc++;
}

/* Or register with A - Affects: S Z A P C */
void instr_orar(I8080_State *s) {
	DBG(s, "Instruction: orar\r\n");
	s->regs[REG_A] |= s->regs[s->mem[s->pc++] & 0x03];
	/* Reset Carry and Aux Carry */
	COND_FLAG(0, s, FLG_C);
	COND_FLAG(0, s, FLG_A);
	gen_pzs(s);
}

/* Or memory with A - Affects: S Z A P C */
void instr_oram(I8080_State *s) {
	DBG(s, "Instruction: oram\r\n");
	s->regs[REG_A] |= s->mem[RP_HL(s)];
	/* Reset Carry and Aux Carry */
	COND_FLAG(0, s, FLG_C);
	COND_FLAG(0, s, FLG_A);
	gen_pzs(s);
	s->pc++;
}

/* Or immediate with A - Affects: S Z A P C */
void instr_ori(I8080_State *s) {
	DBG(s, "Instruction: ori\r\n");
	s->regs[REG_A] |= s->mem[++s->pc];
	/* Reset Carry and Aux Carry */
	COND_FLAG(0, s, FLG_C);
	COND_FLAG(0, s, FLG_A);
	gen_pzs(s);
	s->pc++;
}

/* Rotate A left through carry - Affects: C */
void instr_ral(I8080_State *s) {
	uint8_t res = s->regs[REG_A] << 1;
	DBG(s, "Instruction: ral\r\n");
	res |= FLAG(s, FLG_C);
	COND_FLAG(s->regs[REG_A] & 0x80, s, FLG_C);
	s->regs[REG_A] = res;
	s->pc++;
}

/* Rotate A right through carry - Affects: C */
void instr_rar(I8080_State *s) {
	uint8_t res = s->regs[REG_A] >> 1;
	DBG(s, "Instruction: rar\r\n");
	res |= FLAG(s, FLG_C) << 7;
	COND_FLAG(s->regs[REG_A] & 0x1, s, FLG_C);
	s->regs[REG_A] = res;
	s->pc++;
}

/* Rotate A Left - Affects: C */
void instr_rlc(I8080_State *s) {
	uint8_t res = s->regs[REG_A] << 1;
	DBG(s, "Instruction: rlc\r\n");
	COND_FLAG(s->regs[REG_A] & 0x80, s, FLG_C);
	res |= FLAG(s, FLG_C);
	s->regs[REG_A] = res;
	s->pc++;
}

/* Rotate A Right - Affects: C */
void instr_rrc(I8080_State *s) {
	uint8_t res = s->regs[REG_A] >> 1;
	DBG(s, "Instruction: rrc\r\n");
	COND_FLAG(s->regs[REG_A] & 0x1, s, FLG_C);
	res |= FLAG(s, FLG_C) << 7;
	s->regs[REG_A] = res;
	s->pc++;
}

/* XOR register with A - Affects: S Z A P C */
void instr_xrar(I8080_State *s) {
	DBG(s, "Instruction: xrar\r\n");
	s->regs[REG_A] ^= s->regs[s->mem[s->pc++] & 0x03];
	/* Reset Carry and Aux Carry */
	COND_FLAG(0, s, FLG_C);
	COND_FLAG(0, s, FLG_A);
	gen_pzs(s);
}

/* XOR memory with A - Affects: S Z A P C */
void instr_xram(I8080_State *s) {
	DBG(s, "Instruction: xram\r\n");
	s->regs[REG_A] ^= s->mem[RP_HL(s)];
	/* Reset Carry and Aux Carry */
	COND_FLAG(0, s, FLG_C);
	COND_FLAG(0, s, FLG_A);
	gen_pzs(s);
	s->pc++;
}

/* XOR immediate with A -  Affects: S Z A P C */
void instr_xri(I8080_State *s) {
	DBG(s, "Instruction: xri\r\n");
	s->regs[REG_A] ^= s->mem[++s->pc];
	/* Reset Carry and Aux Carry */
	COND_FLAG(0, s, FLG_C);
	COND_FLAG(0, s, FLG_A);
	gen_pzs(s);
	s->pc++;
}

