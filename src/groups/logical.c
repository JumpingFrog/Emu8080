#include <i8080.h>
#include <groups/logical.h>

/* And register with A - Affects: S Z A P C */
void instr_anar(I8080_State *s) {
	uint8_t r = s->regs[s->mem[s->pc++] & 0x03];
	DBG("Instruction: anar\r\n");
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
	DBG("Instruction: anam\r\n");
	COND_FLAG((r | s->regs[REG_A]) & 0x03, s, FLG_A);
	COND_FLAG(0, s, FLG_C);
	s->regs[REG_A] &= r;
	gen_pzs(s);
	s->pc++;
}

/* And immediate with A - Affects: S Z A P C */
void instr_ani(I8080_State *s) {
	uint8_t r = s->mem[++s->pc];
	DBG("Instruction: ani\r\n");
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
	DBG("Instruction: cmpr\r\n");
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
	DBG("Instruction: cmpm\r\n");
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
	DBG("Instruction: cpi\r\n");
	/* Carry */
	COND_FLAG(!(res & 0x100), s, FLG_C);
	/* Aux Carry */
	GEN_AC(s->regs[REG_A], r, s);
	gen_pzs(s);
	s->pc++;
}

