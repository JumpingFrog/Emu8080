#include <i8080.h>
#include <groups/data.h>

/* Move Immediate Register */
void instr_mvir(I8080_State *s) {
	uint8_t r = (s->mem[s->pc++] & 0x38) >> 3;
	s->regs[r] = s->mem[s->pc++];
}

/* Move Immediate Memory (HL) */
void instr_mvim(I8080_State *s) {
	s->pc++;
	s->mem[RP_HL(s)] = s->mem[s->pc++];
}

/* Move register to register */
void instr_movrr(I8080_State *s) {
	/* Dest */
	uint8_t r1 = (s->mem[s->pc] & 0x38) >> 3;
	/* Src */
	uint8_t r2 = (s->mem[s->pc++] & 0x07);
	s->regs[r1] = s->regs[r2];
}

/* Move register to memory */
void instr_movmr(I8080_State *s) {
	uint8_t r = (s->mem[s->pc++] & 0x07);
	s->mem[RP_HL(s)] = s->regs[r];
}

/* Move memory to register */
void instr_movrm(I8080_State *s) {
	uint8_t r = (s->mem[s->pc++] & 0x38) >> 3;
	s->regs[r] = s->mem[RP_HL(s)];
}

/* Load immediate register pair B, C */
void instr_lxib(I8080_State *s) {
	s->pc++;
	s->regs[REG_C] = s->mem[s->pc++];
	s->regs[REG_B] = s->mem[s->pc++];
}

/* Load immediate register pair D, E */
void instr_lxid(I8080_State *s) {
	s->pc++;
	s->regs[REG_E] = s->mem[s->pc++];
	s->regs[REG_D] = s->mem[s->pc++];
}

/* Load immediate register pair H, L */
void instr_lxih(I8080_State *s) {
	s->pc++;
	s->regs[REG_L] = s->mem[s->pc++];
	s->regs[REG_H] = s->mem[s->pc++];
}

/* Store A indirect */
void instr_staxb(I8080_State *s) {
	s->pc++;
	s->mem[(s->regs[REG_B] << 8) | s->regs[REG_C]] = s->regs[REG_A];
}

/* Store A indirect */
void instr_staxd(I8080_State *s) {
	s->pc++;
	s->mem[(s->regs[REG_D] << 8) | s->regs[REG_E]] = s->regs[REG_A];
}

/* Load A indirect */
void instr_ldaxb(I8080_State *s) {
	s->pc++;
	s->regs[REG_A] = s->mem[(s->regs[REG_B] << 8) | s->regs[REG_C]];
}

/* Load A indirect */
void instr_ldaxd(I8080_State *s) {
	s->pc++;
	s->regs[REG_A] = s->mem[(s->regs[REG_D] << 8) | s->regs[REG_E]];
}

/* Store A direct */
void instr_sta(I8080_State *s) {
	uint16_t addr = s->mem[++s->pc];
	addr |= s->mem[++s->pc] << 8;
	s->pc++;
	s->mem[addr] = s->regs[REG_A];
}

/* Load A direct */
void instr_lda(I8080_State *s) {
	uint16_t addr = s->mem[++s->pc];
	addr |= s->mem[++s->pc] << 8;
	s->pc++;
	s->regs[REG_A] = s->mem[addr];
}

/* Store H, L direct */
void instr_shld(I8080_State *s) {
	uint16_t addr = s->mem[++s->pc] | s->mem[++s->pc] << 8;
	s->pc++;
	s->mem[addr++] = s->regs[REG_L];
	s->mem[addr] = s->regs[REG_H];
}

/* Load H, L direct */
void instr_lhld(I8080_State *s) {
	uint16_t addr = s->mem[++s->pc] | s->mem[++s->pc] << 8;
	s->pc++;
	s->regs[REG_L] = s->mem[addr++];
	s->regs[REG_H] = s->mem[addr];
}

/* Exchange D, E with H, L */
void instr_xchg(I8080_State *s) {
	uint8_t temp = s->regs[REG_D];
	s->regs[REG_D] = s->regs[REG_H];
	s->regs[REG_H] = temp;
	temp =  s->regs[REG_E];
	s->regs[REG_E] = s->regs[REG_L];
	s->regs[REG_L] = temp;
	s->pc++;
}
