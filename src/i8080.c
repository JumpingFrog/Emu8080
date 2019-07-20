#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <i8080.h>
/* Instruction groups */
#include <groups/arithmetic.h>
#include <groups/data.h>
#include <groups/jump.h>
#include <groups/logical.h>
#include <groups/special.h>
#include <groups/stack.h>

/* Lookup instructions for disassemble.
http://pastraiser.com/cpu/i8080/i8080_opcodes.html
*/
static const char *instr_disas[] =
	{
		"nop", "lxi b, ", "stax b", "inx b",
		"inr b", "dcr b", "mvi b, ", "rlc",
		"nop", "dad b", "ldax b", "dcx b",
		"inr c", "dcr c", "mvi c, ", "rrc",
		"nop", "lxi d, ", "stax d", "inx d",
		"inr d", "dcr d", "mvi d, ", "ral",
		"nop", "dad d", "ldax d", "dcx d",
		"inr e", "dcr e", "mvi e, ", "rar",
		"nop", "lxi h, ", "shld ", "inx h",
		"inr h", "dcr h", "mvi h, ", "daa",
		"nop", "dad h", "lhld ", "dcx h",
		"inr l", "dcr l", "mvi l, ", " cma",
		"nop", "lxi sp, ", "sta ", "inx sp",
		"inr m", "dcr m", "mvi m, ", "stc",
		"nop", "dad sp", "lda ", "dcx sp",
		"inr a", "dcr a", "mvi a, ", "cmc",
		"mov b, b", "mov b, c", "mov b, d", "mov b, e",
		"mov b, h", "mov b, l", "mov b, m", "mov b, a",
		"mov c, b", "mov c, c", "mov c, d", "mov c, e",
		"mov c, h", "mov c, l", "mov c, m", "mov c, a",
		"mov d, b", "mov d, c", "mov d, d", "mov d, e",
		"mov d, h", "mov d, l", "mov d, m", "mov d, a",
		"mov e, b", "mov e, c", "mov e, d", "mov e, e",
		"mov e, h", "mov e, l", "mov e, m", "mov e, a",
		"mov h, b", "mov h, c", "mov h, d", "mov h, e",
		"mov h, h", "mov h, l", "mov h, m", "mov h, a",
		"mov l, b", "mov l, c", "mov l, d", "mov l, e",
		"mov l, h", "mov l, l", "mov l, m", "mov l, a",
		"mov m, b", "mov m, c", "mov m, d", "mov m, e",
		"mov m, h", "mov m, l", "hlt", "mov m, a",
		"mov a, b", "mov a, c", "mov a, d", "mov a, e",
		"mov a, h", "mov a, l", "mov a, m", "mov a, a",
		"add b", "add c", "add d", "add e",
		"add h", "add l", "add m", "add a",
		"adc b", "adc c", "adc d", "adc e",
		"adc h", "adc l", "adc m", "adc a",
		"sub b", "sub c", "sub d", "sub e",
		"sub h", "sub l", "sub m", "sub a",
		"sbb b", "sbb c", "sbb d", "sbb e",
		"sbb h", "sbb l", "sbb m", "sbb a",
		"ana b", "ana c", "ana d", "ana e",
		"ana h", "ana l", "ana m", "ana a",
		"xra b", "xra c", "xra d", "xra e",
		"xra h", "xra l", "xra m", "xra a",
		"ora b", "ora c", "ora d", "ora e",
		"ora h", "ora l", "ora m", "ora a",
		"cmp b", "cmp c", "cmp d", "cmp e",
		"cmp h", "cmp l", "cmp m", "cmp a",
		"rnz", "pop b", "jnz ", "jmp ",
		"cnz ", "push b", "adi ", "rst 0",
		"rz", "ret", "jz ", "jmp ",
		"cz ", "call ", "aci ", "rst 1",
		"rnc", "pop d", "jnc ", "out ",
		"cnc ", "push d", "sui ", "rst 2",
		"rc", "ret", "jc ", "in ",
		"cc ", "call ", "sbi ", "rst 3",
		"rpo", "pop h", "jpo ", "xthl",
		"cpo ", "push h", "ani ", "rst 4",
		"rpe", "pchl", "jpe ", "xchg",
		"cpe ", "call ", "xri ", "rst 5",
		"rp", "pop psw", "jp ", "di",
		"cp ", "push psw", "ori ", "rst 6",
		"rm", "sphl", "jm ", "ei",
		"cm ", "call ", "cpi ", "rst 7"
};

/* Instruction decode table. */
static const Instruction instr_decode[] =
	{
		instr_nop, instr_lxib, instr_staxb, instr_inxb, /*0x03*/
		instr_inrr, instr_dcrr, instr_mvir, instr_rlc, /*0x07*/
		instr_nop, instr_dadb, instr_ldaxb, instr_dcxb, /*0x0B*/
		instr_inrr, instr_dcrr, instr_mvir, instr_rrc, /*0x0F*/
		instr_nop, instr_lxid, instr_staxd, instr_inxd, /*0x13*/
		instr_inrr, instr_dcrr, instr_mvir, instr_ral, /*0x17*/
		instr_nop, instr_dadd, instr_ldaxd, instr_dcxd, /*0x1B*/
		instr_inrr, instr_dcrr, instr_mvir, instr_rar, /*0x1F*/
		instr_nop, instr_lxih, instr_shld, instr_inxh, /*0x23*/
		instr_inrr, instr_dcrr, instr_mvir, instr_daa, /*0x27*/
		instr_nop, instr_dadh, instr_lhld, instr_dcxh, /*0x2B*/
		instr_inrr, instr_dcrr, instr_mvir, instr_cma, /*0x2F*/
		instr_nop, instr_lxisp, instr_sta, instr_inxs, /*0x33*/
		instr_inrm, instr_dcrm, instr_mvim, instr_stc, /*0x37*/
		instr_nop, instr_dads, instr_lda, instr_dcxs, /*0x3B*/
		instr_inrr, instr_dcrr, instr_mvir, instr_cmc, /*0x3F*/
		instr_movrr, instr_movrr, instr_movrr, instr_movrr, /*0x43*/
		instr_movrr, instr_movrr, instr_movrm, instr_movrr, /*0x47*/
		instr_movrr, instr_movrr, instr_movrr, instr_movrr, /*0x4B*/
		instr_movrr, instr_movrr, instr_movrm, instr_movrr, /*0x4F*/
		instr_movrr, instr_movrr, instr_movrr, instr_movrr, /*0x53*/
		instr_movrr, instr_movrr, instr_movrm, instr_movrr, /*0x57*/
		instr_movrr, instr_movrr, instr_movrr, instr_movrr, /*0x5B*/
		instr_movrr, instr_movrr, instr_movrm, instr_movrr, /*0x5F*/
		instr_movrr, instr_movrr, instr_movrr, instr_movrr, /*0x63*/
		instr_movrr, instr_movrr, instr_movrm, instr_movrr, /*0x67*/
		instr_movrr, instr_movrr, instr_movrr, instr_movrr, /*0x6B*/
		instr_movrr, instr_movrr, instr_movrm, instr_movrr, /*0x6F*/
		instr_movmr, instr_movmr, instr_movmr, instr_movmr, /*0x73*/
		instr_movmr, instr_movmr, instr_hlt, instr_movmr, /*0x77*/
		instr_movrr, instr_movrr, instr_movrr, instr_movrr, /*0x7B*/
		instr_movrr, instr_movrr, instr_movrm, instr_movrr, /*0x7F*/
		instr_addr, instr_addr, instr_addr, instr_addr, /*0x83*/
		instr_addr, instr_addr, instr_addm, instr_addr, /*0x87*/
		instr_adcr, instr_adcr, instr_adcr, instr_adcr, /*0x8B*/
		instr_adcr, instr_adcr, instr_adcm, instr_adcr, /*0x8F*/
		instr_subr, instr_subr, instr_subr, instr_subr, /*0x93*/
		instr_subr, instr_subr, instr_subm, instr_subr, /*0x97*/
		instr_sbbr, instr_sbbr, instr_sbbr, instr_sbbr, /*0x9B*/
		instr_sbbr, instr_sbbr, instr_sbbm, instr_sbbr, /*0x9F*/
		instr_anar, instr_anar, instr_anar, instr_anar, /*0xA3*/
		instr_anar, instr_anar, instr_anam, instr_anar, /*0xA7*/
		instr_xrar, instr_xrar, instr_xrar, instr_xrar, /*0xAB*/
		instr_xrar, instr_xrar, instr_xram, instr_xrar, /*0xAF*/
		instr_orar, instr_orar, instr_orar, instr_orar, /*0xB3*/
		instr_orar, instr_orar, instr_oram, instr_orar, /*0xB7*/
		instr_cmpr, instr_cmpr, instr_cmpr, instr_cmpr, /*0xBB*/
		instr_cmpr, instr_cmpr, instr_cmpm, instr_cmpr, /*0xBF*/
		instr_rnz, instr_popb, instr_jnz, instr_jmp, /*0xC3*/
		instr_cnz, instr_pushb, instr_adi, instr_rst, /*0xC7*/
		instr_rz, instr_ret, instr_jz, instr_jmp, /*0xCB*/
		instr_cz, instr_call, instr_aci, instr_rst, /*0xCF*/
		instr_rnc, instr_popd, instr_jnc, instr_out, /*0xD3*/
		instr_cnc, instr_pushd, instr_sui, instr_rst, /*0xD7*/
		instr_rc, instr_nop, instr_jc, instr_in, /*0xDB*/
		instr_cc, instr_nop, instr_sbi, instr_rst, /*0xDF*/
		instr_rpo, instr_poph, instr_jpo, instr_xthl, /*0xE3*/
		instr_cpo, instr_pushh, instr_ani, instr_rst, /*0xE7*/
		instr_rpe, instr_pchl, instr_jpe, instr_xchg, /*0xEB*/
		instr_cpe, instr_nop, instr_xri, instr_rst, /*0xEF*/
		instr_rp, instr_popp, instr_jp, instr_di, /*0xF3*/
		instr_cp, instr_pushp, instr_ori, instr_rst, /*0xF7*/
		instr_rm, instr_sphl, instr_jm, instr_ei, /*0xFB*/
		instr_cm, instr_nop, instr_cpi, instr_rst  /*0xFF*/
};

static const uint8_t instr_length[] =
	{
		1, 3, 1, 1, /*0x03*/
		1, 1, 2, 1, /*0x07*/
		1, 1, 1, 1, /*0x0B*/
		1, 1, 2, 1, /*0x0F*/
		1, 3, 1, 1, /*0x13*/
		1, 1, 2, 1, /*0x17*/
		1, 1, 1, 1, /*0x1B*/
		1, 1, 2, 1, /*0x1F*/
		1, 3, 3, 1, /*0x23*/
		1, 1, 2, 1, /*0x27*/
		1, 1, 3, 1, /*0x2B*/
		1, 1, 2, 1, /*0x2F*/
		1, 3, 3, 1, /*0x33*/
		1, 1, 2, 1, /*0x37*/
		1, 1, 3, 1, /*0x3B*/
		1, 1, 2, 1, /*0x3F*/
		1, 1, 1, 1, /*0x43*/
		1, 1, 1, 1, /*0x47*/
		1, 1, 1, 1, /*0x4B*/
		1, 1, 1, 1, /*0x4F*/
		1, 1, 1, 1, /*0x53*/
		1, 1, 1, 1, /*0x57*/
		1, 1, 1, 1, /*0x5B*/
		1, 1, 1, 1, /*0x5F*/
		1, 1, 1, 1, /*0x63*/
		1, 1, 1, 1, /*0x67*/
		1, 1, 1, 1, /*0x6B*/
		1, 1, 1, 1, /*0x6F*/
		1, 1, 1, 1, /*0x73*/
		1, 1, 1, 1, /*0x77*/
		1, 1, 1, 1, /*0x7B*/
		1, 1, 1, 1, /*0x7F*/
		1, 1, 1, 1, /*0x83*/
		1, 1, 1, 1, /*0x87*/
		1, 1, 1, 1, /*0x8B*/
		1, 1, 1, 1, /*0x8F*/
		1, 1, 1, 1, /*0x93*/
		1, 1, 1, 1, /*0x97*/
		1, 1, 1, 1, /*0x9B*/
		1, 1, 1, 1, /*0x9F*/
		1, 1, 1, 1, /*0xA3*/
		1, 1, 1, 1, /*0xA7*/
		1, 1, 1, 1, /*0xAB*/
		1, 1, 1, 1, /*0xAF*/
		1, 1, 1, 1, /*0xB3*/
		1, 1, 1, 1, /*0xB7*/
		1, 1, 1, 1, /*0xBB*/
		1, 1, 1, 1, /*0xBF*/
		1, 1, 3, 3, /*0xC3*/
		3, 1, 2, 1, /*0xC7*/
		1, 1, 3, 3, /*0xCB*/
		3, 3, 2, 1, /*0xCF*/
		1, 1, 3, 2, /*0xD3*/
		3, 1, 2, 1, /*0xD7*/
		1, 1, 3, 2, /*0xDB*/
		3, 3, 2, 1, /*0xDF*/
		1, 1, 3, 1, /*0xE3*/
		3, 1, 2, 1, /*0xE7*/
		1, 1, 3, 1, /*0xEB*/
		3, 3, 2, 1, /*0xEF*/
		1, 1, 3, 1, /*0xF3*/
		3, 1, 2, 1, /*0xF7*/
		1, 1, 3, 1, /*0xFB*/
		3, 3, 2, 1  /*0xFF*/
};

I8080_State *init_8080() {
	I8080_State * ret = malloc(sizeof(I8080_State));
	ret->pc = 0x0000;
	ret->flags = 0x02;
	ret->sp = 0xFFFF;
	ret->hlt = 0;
	ret->reg_mod = 0;
	#ifdef TRACE_FILE
		ret->ftrace = fopen("trace.log", "w+");
	#endif
	return ret;
}

/* A LUT may be faster still. */
inline void gen_p(I8080_State * s, uint8_t value) {
	/* Parity */
	value ^= value >> 4;
	value ^= value >> 2;
	value ^= value >> 1;
	COND_FLAG(s, value & 1, FLG_P);
}

void dbg_8080(I8080_State *s) {
	uint8_t opcode = s->mem[s->pc];
	TRACE(s, "-------------------------------------------------");
	TRACEF(s, "PC: 0x%04x \t [PC]: 0x%02x \t ", s->pc, opcode);
	switch (instr_length[opcode]) {
		case 1:
			TRACEF(s, "OP: %s\n", instr_disas[opcode]);
			break;
		case 2:
			TRACEF(s, "OP: %s0x%02x\n", instr_disas[opcode], s->mem[s->pc + 1]);
			break;
		case 3:
			/* N.B. LE */
			TRACEF(s, "OP: %s0x%02x%02x\n", instr_disas[opcode], s->mem[s->pc + 2], s->mem[s->pc + 1]);
			break;
		default:
			TRACEF(s, "OP: ERROR\n");
	}
	/* Print modified state */
	/* |SP (9)|flags (8)|REG_A (7)|REG_M (6)|REG_L (5)|REG_H (4)|REG_E (3)|REG_D (2)|REG_C (1)|REG_B (0)| */
	if (s->reg_mod & MOD_REG(REG_A)) {
		TRACEF(s, "A: 0x%02x\n", s->regs[REG_A]);
	}
	if (s->reg_mod & MOD_REG(REG_B)) {
		TRACEF(s, "B: 0x%02x\n", s->regs[REG_B]);
	}
	if (s->reg_mod & MOD_REG(REG_C)) {
		TRACEF(s, "C: 0x%02x\n", s->regs[REG_C]);
	}
	if (s->reg_mod & MOD_REG(REG_D)) {
		TRACEF(s, "D: 0x%02x\n", s->regs[REG_D]);
	}
	if (s->reg_mod & MOD_REG(REG_E)) {
		TRACEF(s, "E: 0x%02x\n", s->regs[REG_E]);
	}
	if (s->reg_mod & MOD_REG(REG_H)) {
		TRACEF(s, "H: 0x%02x\n", s->regs[REG_H]);
	}
	if (s->reg_mod & MOD_REG(REG_L)) {
		TRACEF(s, "L: 0x%02x\n", s->regs[REG_L]);
	}
	if (s->reg_mod & MOD_REG(REG_M)) {
		TRACEF(s, "M:  0x%04x \t [M]: 0x%02x\n",
			 RP_HL(s),  s->mem[RP_HL(s)]);
	}
	if (s->reg_mod & MOD_FLAGS) {
		TRACEF(s, "S: %x \t Z: %x \t A: %x \t P: %x \t C: %x\n",
				READ_FLAG(s, FLG_S), READ_FLAG(s, FLG_Z), READ_FLAG(s, FLG_A),
				READ_FLAG(s, FLG_P), READ_FLAG(s, FLG_C));
	}
	if (s->reg_mod & MOD_SP) {
		TRACEF(s, "SP: 0x%04x \t [SP]: 0x%02x \t [SP+1]: 0x%02x\n",
				s->sp, s->mem[s->sp], s->mem[s->sp + 1]);
	}
	/* TODO: MEMORY WRITES */
}

void add_dev_8080(I8080_State *s, uint8_t addr, IODevice *dev) {
	s->devices[addr] = dev;
}

void run_8080(I8080_State *s) {
	uint8_t opcode;
	while (!s->hlt) {
		/* Clear mod vector */
		s->reg_mod = 0;
		/* Execute current opcode */
		opcode = s->mem[s->pc];
		(*instr_decode[opcode])(s);
		dbg_8080(s);
		/* Move PC on by instr length */
		s->pc += instr_length[opcode];
	}
	#ifdef TRACE_FILE
		fclose(s->ftrace);
	#endif
}

