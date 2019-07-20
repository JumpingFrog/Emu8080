#include <i8080.h>
#include <groups/data.h>

/* Move Immediate Register - Affects: None */
void instr_mvir(I8080_State *s) {
	uint8_t r = (s->mem[s->pc] & 0x38) >> 3;
	DBG(s, "Instruction: mvir\n");
	WRITE_REG(s, r,  s->mem[s->pc + 1]);
}

/* Move Immediate Memory (HL) - Affects: None */
void instr_mvim(I8080_State *s) {
	DBG(s, "Instruction: mvim\n");
	WRITE_MEM(s, RP_HL(s), s->mem[s->pc + 1]);
}

/* Move register to register  - Affects: None */
void instr_movrr(I8080_State *s) {
	/* Dest */
	uint8_t r1 = (s->mem[s->pc] & 0x38) >> 3;
	/* Src */
	uint8_t r2 = (s->mem[s->pc] & 0x07);
	DBG(s, "Instruction: movrr\n");
	WRITE_REG(s, r1, s->regs[r2]);
}

/* Move register to memory  - Affects: None */
void instr_movmr(I8080_State *s) {
	uint8_t r = (s->mem[s->pc] & 0x07);
	DBG(s, "Instruction: movmr\n");
	WRITE_MEM(s, RP_HL(s), s->regs[r]);
}

/* Move memory to register  - Affects: None */
void instr_movrm(I8080_State *s) {
	uint8_t r = (s->mem[s->pc] & 0x38) >> 3;
	DBG(s, "Instruction: movrm\n");
	WRITE_REG(s, r, s->mem[RP_HL(s)]);
}

/* Load immediate register pair B, C  - Affects: None */
void instr_lxib(I8080_State *s) {
	DBG(s, "Instruction: lxib\n");
	WRITE_REG(s, REG_C, s->mem[s->pc + 1]);
	WRITE_REG(s, REG_B, s->mem[s->pc + 2]);
}

/* Load immediate register pair D, E  - Affects: None */
void instr_lxid(I8080_State *s) {
	DBG(s, "Instruction: lxid\n");
	WRITE_REG(s, REG_E, s->mem[s->pc + 1]);
	WRITE_REG(s, REG_D, s->mem[s->pc + 2]);
}

/* Load immediate register pair H, L  - Affects: None */
void instr_lxih(I8080_State *s) {
	DBG(s, "Instruction: lxih\n");
	WRITE_REG(s, REG_L, s->mem[s->pc + 1]);
	WRITE_REG(s, REG_H, s->mem[s->pc + 2]);
}

/* Load immediate SP  - Affects: None */
void instr_lxisp(I8080_State *s) {
	DBG(s, "Instruction: lxisp\n");
	uint16_t res = (s->mem[s->pc + 2] << 8) | s->mem[s->pc + 1];
	WRITE_SP(s, res);
}

/* Store A indirect - Affects: None */
void instr_staxb(I8080_State *s) {
	DBG(s, "Instruction: staxb\n");
	WRITE_MEM(s, RP_BC(s), s->regs[REG_A]);
}

/* Store A indirect - Affects: None */
void instr_staxd(I8080_State *s) {
	DBG(s, "Instruction: staxd\n");
	WRITE_MEM(s, RP_DE(s), s->regs[REG_A]);
}

/* Load A indirect - Affects: None */
void instr_ldaxb(I8080_State *s) {
	DBG(s, "Instruction: ldaxb\n");
	WRITE_REG(s, REG_A, s->mem[RP_BC(s)]);
}

/* Load A indirect - Affects: None */
void instr_ldaxd(I8080_State *s) {
	DBG(s, "Instruction: ldaxd\n");
	WRITE_REG(s, REG_A, s->mem[RP_DE(s)]);
}

/* Store A direct - Affects: None */
void instr_sta(I8080_State *s) {
	uint16_t addr = s->mem[s->pc + 2] << 8 | s->mem[s->pc + 1];
	DBG(s, "Instruction: sta\n");
	WRITE_MEM(s, addr, s->regs[REG_A]);
}

/* Load A direct - Affects: None */
void instr_lda(I8080_State *s) {
	uint16_t addr = s->mem[s->pc + 2] << 8 | s->mem[s->pc + 1];
	DBG(s, "Instruction: lda\n");
	WRITE_REG(s, REG_A, s->mem[addr]);
}

/* Store H, L direct - Affects: None */
void instr_shld(I8080_State *s) {
	uint16_t addr = s->mem[s->pc + 2] << 8 | s->mem[s->pc + 1];
	DBG(s, "Instruction: shld\n");
	WRITE_MEM(s, addr, s->regs[REG_H]);
	WRITE_MEM(s, addr + 1, s->regs[REG_L]);
}

/* Load H, L direct - Affects: None */
void instr_lhld(I8080_State *s) {
	uint16_t addr = s->mem[s->pc + 2] << 8 | s->mem[s->pc + 1];
	DBG(s, "Instruction: lhld\n");
	WRITE_REG(s, REG_L, s->mem[addr + 1]);
	WRITE_REG(s, REG_H, s->mem[addr]);
}

/* Exchange D, E with H, L - Affects: None */
void instr_xchg(I8080_State *s) {
	uint8_t temp = s->regs[REG_D];
	DBG(s, "Instruction: xchg\n");
	WRITE_REG(s, REG_D, s->regs[REG_H]);
	WRITE_REG(s, REG_H, temp);
	temp =  s->regs[REG_E];
	WRITE_REG(s, REG_E, s->regs[REG_L]);
	WRITE_REG(s, REG_L, temp);
}

