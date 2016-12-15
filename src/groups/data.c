#include <i8080.h>
#include <groups/data.h>

/* Move Immediate Register */
void instr_mvir(I8080_State *s) {
	uint8_t r = (s->mem[s->pc++] & 0x38) >> 3;
	DBG(s, "Instruction: mvir\r\n");
	s->regs[r] = s->mem[s->pc++];
}

/* Move Immediate Memory (HL) */
void instr_mvim(I8080_State *s) {
	DBG(s, "Instruction: mvim\r\n");
	mem_write8(s, RP_HL(s), s->mem[++s->pc]);
	s->pc++;
}

/* Move register to register */
void instr_movrr(I8080_State *s) {
	/* Dest */
	uint8_t r1 = (s->mem[s->pc] & 0x38) >> 3;
	/* Src */
	uint8_t r2 = (s->mem[s->pc++] & 0x07);
	DBG(s, "Instruction: movrr\r\n");
	s->regs[r1] = s->regs[r2];
}

/* Move register to memory */
void instr_movmr(I8080_State *s) {
	uint8_t r = (s->mem[s->pc++] & 0x07);
	DBG(s, "Instruction: movmr\r\n");
	mem_write8(s, RP_HL(s), s->regs[r]);
}

/* Move memory to register */
void instr_movrm(I8080_State *s) {
	uint8_t r = (s->mem[s->pc++] & 0x38) >> 3;
	DBG(s, "Instruction: movrm\r\n");
	s->regs[r] = mem_read8(s, RP_HL(s));
}

/* Load immediate register pair B, C */
void instr_lxib(I8080_State *s) {
	DBG(s, "Instruction: lxib\r\n");
	s->regs[REG_C] = s->mem[++s->pc];
	s->regs[REG_B] = s->mem[++s->pc];
	s->pc++;
}

/* Load immediate register pair D, E */
void instr_lxid(I8080_State *s) {
	DBG(s, "Instruction: lxid\r\n");
	s->regs[REG_E] = s->mem[++s->pc];
	s->regs[REG_D] = s->mem[++s->pc];
	s->pc++;
}

/* Load immediate register pair H, L */
void instr_lxih(I8080_State *s) {
	DBG(s, "Instruction: lxih\r\n");
	s->regs[REG_L] = s->mem[++s->pc];
	s->regs[REG_H] = s->mem[++s->pc];
	s->pc++;
}

/* Load immediate SP */
void instr_lxisp(I8080_State *s) {
	DBG(s, "Instruction: lxisp\r\n");
	s->sp = s->mem[++s->pc];
	s->sp |= s->mem[++s->pc] << 0x08;
	s->pc++;
}

/* Store A indirect */
void instr_staxb(I8080_State *s) {
	DBG(s, "Instruction: staxb\r\n");
	mem_write8(s, RP_BC(s), s->regs[REG_A]);
	s->pc++;
}

/* Store A indirect */
void instr_staxd(I8080_State *s) {
	DBG(s, "Instruction: staxd\r\n");
	mem_write8(s, RP_DE(s), s->regs[REG_A]);
	s->pc++;
}

/* Load A indirect */
void instr_ldaxb(I8080_State *s) {
	DBG(s, "Instruction: ldaxb\r\n");
	s->regs[REG_A] = mem_read8(s, RP_BC(s));
	s->pc++;
}

/* Load A indirect */
void instr_ldaxd(I8080_State *s) {
	DBG(s, "Instruction: ldaxd\r\n");
	s->regs[REG_A] = mem_read8(s, RP_DE(s));
	s->pc++;
}

/* Store A direct */
void instr_sta(I8080_State *s) {
	uint16_t addr = s->mem[++s->pc];
	DBG(s, "Instruction: sta\r\n");
	addr |= s->mem[++s->pc] << 8;
	mem_write8(s, addr, s->regs[REG_A]);
	s->pc++;
}

/* Load A direct */
void instr_lda(I8080_State *s) {
	uint16_t addr = s->mem[++s->pc];
	DBG(s, "Instruction: lda\r\n");
	addr |= s->mem[++s->pc] << 8;
	s->regs[REG_A] = mem_read8(s, addr);
	s->pc++;
}

/* Store H, L direct */
void instr_shld(I8080_State *s) {
	uint16_t addr = s->mem[++s->pc] | s->mem[++s->pc] << 8;
	DBG(s, "Instruction: shld\r\n");
	mem_write16(s, addr, RP_HL(s));
	s->pc++;
}

/* Load H, L direct */
void instr_lhld(I8080_State *s) {
	uint16_t addr = s->mem[++s->pc] | s->mem[++s->pc] << 8;
	uint16_t val = mem_read16(s, addr);
	DBG(s, "Instruction: lhld\r\n");
	s->regs[REG_L] = val & 0xFF;
	s->regs[REG_H] = (val >> 8) & 0xFF;
	s->pc++;
}

/* Exchange D, E with H, L */
void instr_xchg(I8080_State *s) {
	uint8_t temp = s->regs[REG_D];
	DBG(s, "Instruction: xchg\r\n");
	s->regs[REG_D] = s->regs[REG_H];
	s->regs[REG_H] = temp;
	temp =  s->regs[REG_E];
	s->regs[REG_E] = s->regs[REG_L];
	s->regs[REG_L] = temp;
	s->pc++;
}

