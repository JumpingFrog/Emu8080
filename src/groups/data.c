#include <i8080.h>
#include <groups/data.h>

/* Move Immediate Register - Affects: None */
void instr_mvir(I8080State *s) {
	uint8_t r = (READ_OP(s) & 0x38) >> 3;
	DBG(s, "Instruction: mvir\n");
	WRITE_REG(s, r, READ_IMM8(s));
}

/* Move Immediate Memory (HL) - Affects: None */
void instr_mvim(I8080State *s) {
	DBG(s, "Instruction: mvim\n");
	WRITE_MEM(s, RP_HL(s), READ_IMM8(s));
}

/* Move register to register  - Affects: None */
void instr_movrr(I8080State *s) {
	/* Dest */
	uint8_t r1 = (READ_OP(s) & 0x38) >> 3;
	/* Src */
	uint8_t r2 = (READ_OP(s) & 0x07);
	DBG(s, "Instruction: movrr\n");
	WRITE_REG(s, r1, s->regs[r2]);
}

/* Move register to memory  - Affects: None */
void instr_movmr(I8080State *s) {
	uint8_t r = (READ_OP(s) & 0x07);
	DBG(s, "Instruction: movmr\n");
	WRITE_MEM(s, RP_HL(s), s->regs[r]);
}

/* Move memory to register  - Affects: None */
void instr_movrm(I8080State *s) {
	uint8_t r = (READ_OP(s) & 0x38) >> 3;
	DBG(s, "Instruction: movrm\n");
	WRITE_REG(s, r, READ_MEM(s, RP_HL(s)));
}

/* Load immediate register pair B, C  - Affects: None */
void instr_lxib(I8080State *s) {
	DBG(s, "Instruction: lxib\n");
	WRITE_REG(s, REG_C, READ_MEM(s, s->pc + 1));
	WRITE_REG(s, REG_B, READ_MEM(s, s->pc + 2));
}

/* Load immediate register pair D, E  - Affects: None */
void instr_lxid(I8080State *s) {
	DBG(s, "Instruction: lxid\n");
	WRITE_REG(s, REG_E, READ_MEM(s, s->pc + 1));
	WRITE_REG(s, REG_D, READ_MEM(s, s->pc + 2));
}

/* Load immediate register pair H, L  - Affects: None */
void instr_lxih(I8080State *s) {
	DBG(s, "Instruction: lxih\n");
	WRITE_REG(s, REG_L, READ_MEM(s, s->pc + 1));
	WRITE_REG(s, REG_H, READ_MEM(s, s->pc + 2));
}

/* Load immediate SP  - Affects: None */
void instr_lxis(I8080State *s) {
	uint16_t res = READ_IMM16(s);
	DBG(s, "Instruction: lxis\n");
	WRITE_SP(s, res);
}

/* Store A indirect - Affects: None */
void instr_staxb(I8080State *s) {
	DBG(s, "Instruction: staxb\n");
	WRITE_MEM(s, RP_BC(s), s->regs[REG_A]);
}

/* Store A indirect - Affects: None */
void instr_staxd(I8080State *s) {
	DBG(s, "Instruction: staxd\n");
	WRITE_MEM(s, RP_DE(s), s->regs[REG_A]);
}

/* Load A indirect - Affects: None */
void instr_ldaxb(I8080State *s) {
	DBG(s, "Instruction: ldaxb\n");
	WRITE_REG(s, REG_A, READ_MEM(s, RP_BC(s)));
}

/* Load A indirect - Affects: None */
void instr_ldaxd(I8080State *s) {
	DBG(s, "Instruction: ldaxd\n");
	WRITE_REG(s, REG_A, READ_MEM(s, RP_DE(s)));
}

/* Store A direct - Affects: None */
void instr_sta(I8080State *s) {
	DBG(s, "Instruction: sta\n");
	WRITE_MEM(s, READ_IMM16(s), s->regs[REG_A]);
}

/* Load A direct - Affects: None */
void instr_lda(I8080State *s) {
	DBG(s, "Instruction: lda\n");
	WRITE_REG(s, REG_A, READ_MEM(s, READ_IMM16(s)));
}

/* Store H, L direct - Affects: None */
void instr_shld(I8080State *s) {
	uint16_t addr = READ_IMM16(s);
	DBG(s, "Instruction: shld\n");
	WRITE_MEM(s, addr, s->regs[REG_H]);
	WRITE_MEM(s, addr + 1, s->regs[REG_L]);
}

/* Load H, L direct - Affects: None */
void instr_lhld(I8080State *s) {
	uint16_t addr = READ_IMM16(s);
	DBG(s, "Instruction: lhld\n");
	WRITE_REG(s, REG_L, READ_MEM(s, addr + 1));
	WRITE_REG(s, REG_H, READ_MEM(s, addr));
}

/* Exchange D, E with H, L - Affects: None */
void instr_xchg(I8080State *s) {
	uint8_t temp = s->regs[REG_D];
	DBG(s, "Instruction: xchg\n");
	WRITE_REG(s, REG_D, s->regs[REG_H]);
	WRITE_REG(s, REG_H, temp);
	temp =  s->regs[REG_E];
	WRITE_REG(s, REG_E, s->regs[REG_L]);
	WRITE_REG(s, REG_L, temp);
}

