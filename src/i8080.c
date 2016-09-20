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
static const char *opcodes[] =
	{
		"0nop", "2lxi b, ", "0stax b", "0inx b",
		"0inr b", "0dcr b", "1mvi b, ", "0rlc",
		"0nop", "0dad b", "0ldax b", "0dcx b",
		"0inr c", "0dcr c", "1mvi c, ", "0rrc",
		"0nop", "2lxi d, ", "0stax d", "0inx d",
		"0inr d", "0dcr d", "1mvi d, ", "0ral",
		"0nop", "0dad d", "0ldax d", "0dcx d",
		"0inr e", "0dcr e", "1mvi e, ", "0rar",
		"0nop", "2lxi h, ", "2shld ", "0inx h",
		"0inr h", "0dcr h", "1mvi h, ", "0daa",
		"0nop", "0dad h", "2lhld ", "0dcx h",
		"0inr l", "0dcr l", "1mvi l, ", "0 cma",
		"0nop", "2lxi sp, ", "2sta ", "0inx sp",
		"0inr m", "0dcr m", "1mvi m, ", "0stc",
		"0nop", "0dad sp", "2lda ", "0dcx sp",
		"0inr a", "0dcr a", "1mvi a, ", "0cmc",
		"0mov b, b", "0mov b, c", "0mov b, d", "0mov b, e",
		"0mov b, h", "0mov b, l", "0mov b, m", "0mov b, a",
		"0mov c, b", "0mov c, c", "0mov c, d", "0mov c, e",
		"0mov c, h", "0mov c, l", "0mov c, m", "0mov c, a",
		"0mov d, b", "0mov d, c", "0mov d, d", "0mov d, e",
		"0mov d, h", "0mov d, l", "0mov d, m", "0mov d, a",
		"0mov e, b", "0mov e, c", "0mov e, d", "0mov e, e",
		"0mov e, h", "0mov e, l", "0mov e, m", "0mov e, a",
		"0mov h, b", "0mov h, c", "0mov h, d", "0mov h, e",
		"0mov h, h", "0mov h, l", "0mov h, m", "0mov h, a",
		"0mov l, b", "0mov l, c", "0mov l, d", "0mov l, e",
		"0mov l, h", "0mov l, l", "0mov l, m", "0mov l, a",
		"0mov m, b", "0mov m, c", "0mov m, d", "0mov m, e",
		"0mov m, h", "0mov m, l", "0hlt", "0mov m, a",
		"0mov a, b", "0mov a, c", "0mov a, d", "0mov a, e",
		"0mov a, h", "0mov a, l", "0mov a, m", "0mov a, a",
		"0add b", "0add c", "0add d", "0add e",
		"0add h", "0add l", "0add m", "0add a",
		"0adc b", "0adc c", "0adc d", "0adc e",
		"0adc h", "0adc l", "0adc m", "0adc a",
		"0sub b", "0sub c", "0sub d", "0sub e",
		"0sub h", "0sub l", "0sub m", "0sub a",
		"0sbb b", "0sbb c", "0sbb d", "0sbb e",
		"0sbb h", "0sbb l", "0sbb m", "0sbb a",
		"0ana b", "0ana c", "0ana d", "0ana e",
		"0ana h", "0ana l", "0ana m", "0ana a",
		"0xra b", "0xra c", "0xra d", "0xra e",
		"0xra h", "0xra l", "0xra m", "0xra a",
		"0ora b", "0ora c", "0ora d", "0ora e",
		"0ora h", "0ora l", "0ora m", "0ora a",
		"0cmp b", "0cmp c", "0cmp d", "0cmp e",
		"0cmp h", "0cmp l", "0cmp m", "0cmp a",
		"0rnz", "0pop b", "2jnz ", "2jmp ",
		"2cnz ", "0push b", "1adi ", "0rst 0",
		"0rz", "0ret", "2jz ", "2jmp ",
		"2cz ", "2call ", "1aci ", "0rst 1",
		"0rnc", "0pop d", "2jnc ", "1out ",
		"2cnc ", "0push d", "1sui ", "0rst 2",
		"0rc", "0ret", "2jc ", "1in ",
		"2cc ", "2call ", "1sbi ", "0rst 3",
		"0rpo", "0pop h", "2jpo ", "0xthl",
		"2cpo ", "0push h", "1ani ", "0rst 4",
		"0rpe", "0pchl", "2jpe ", "0xchg",
		"2cpe ", "2call ", "1xri ", "0rst 5",
		"0rp", "0pop psw", "2jp ", "0di",
		"2cp ", "0push psw", "1ori ", "0rst 6",
		"0rm", "0sphl", "2jm ", "0ei",
		"2cm ", "2call ", "1cpi ", "0rst 7"
};

/* Instruction decode table. */
static const Instruction decode[] =
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
		instr_nop, instr_dadh, instr_lhld, instr_nop, /*0x2B*/
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
		instr_rc, instr_nop, instr_jc, instr_nop, /*0xDB*/
		instr_cc, instr_nop, instr_sbi, instr_rst, /*0xDF*/
		instr_rpo, instr_poph, instr_jpo, instr_xthl, /*0xE3*/
		instr_cpo, instr_pushh, instr_ani, instr_rst, /*0xE7*/
		instr_rpe, instr_pchl, instr_jpe, instr_xchg, /*0xEB*/
		instr_cpe, instr_nop, instr_xri, instr_rst, /*0xEF*/
		instr_rp, instr_popp, instr_jp, instr_nop, /*0xF3*/
		instr_cp, instr_pushp, instr_ori, instr_rst, /*0xF7*/
		instr_rm, instr_sphl, instr_jm, instr_nop, /*0xFB*/
		instr_cm, instr_nop, instr_cpi, instr_rst  /*0xFF*/
};

I8080_State *init_8080() {
	I8080_State * ret = malloc(sizeof(I8080_State));
	ret->pc = 0x0000;
	ret->flags = 0x02;
	ret->sp = 0xFFFF;
	ret->hlt = 0;
	return ret;
}

/* Generate PZS flags from Reg A */
inline void gen_pzs(I8080_State *s) {
	uint8_t temp = s->regs[REG_A];
	/* Zero */
	COND_FLAG(!temp, s, FLG_Z);
	/* Sign */
	COND_FLAG(temp & 0x80, s, FLG_S);
	/* Parity */
	temp ^= temp >> 4;
	temp ^= temp >> 2;
	temp ^= temp >> 1;
	COND_FLAG(!(temp & 1), s, FLG_P);
}

void dbg_8080(I8080_State *s) {
	const char *opstr = opcodes[s->mem[s->pc]];
	puts("-------------------------------------------------");
	printf("PC: 0x%04x \t [PC]: 0x%02x \t", s->pc, s->mem[s->pc]);
	/* First char of opcode describes number of bytes to instruction. */
	switch (*opstr) {
		case '0':
			opstr++;
			printf("OP: %s\r\n", opstr);
			break;
		case '1':
			opstr++;
			printf("OP: %s0x%02x\r\n", opstr, s->mem[s->pc + 1]);
			break;
		case '2':
			opstr++;
			/* N.B. LE */
			printf("OP: %s0x%02x%02x\r\n", opstr, s->mem[s->pc + 2], s->mem[s->pc + 1]);
			break;
	}
	printf("A:  0x%02x \t F: 0x%02x\r\n"
			"B:  0x%02x \t C: 0x%02x\r\n"
			"D:  0x%02x \t E: 0x%02x\r\n"
			"H:  0x%02x \t L: 0x%02x \t [M]: 0x%02x\r\n"
			"SP: 0x%04x \t [SP]: 0x%02x \t [SP+1]: 0x%02x\r\n"
			"S: %x Z: %x \t A: %x P: %x \t C: %x\r\n",
			s->regs[REG_A], s->flags,
			s->regs[REG_B], s->regs[REG_C],
			s->regs[REG_D], s->regs[REG_E],
			s->regs[REG_H], s->regs[REG_L], s->mem[RP_HL(s)],
			s->sp, s->mem[s->sp], s->mem[s->sp + 1],
			FLAG(s, FLG_S), FLAG(s, FLG_Z), FLAG(s, FLG_A),
			FLAG(s, FLG_P), FLAG(s, FLG_C));
}

void add_dev_8080(I8080_State *s, uint8_t addr, IODevice *dev) {
	s->devices[addr] = dev;
}

/* Run... */
void run_8080(I8080_State *s) {
	while (!s->hlt) {
		dbg_8080(s);
		/* Execute current opcode */
		(*decode[s->mem[s->pc]])(s);
	}
}
