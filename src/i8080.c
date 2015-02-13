#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "i8080.h"
/* Instruction groups */
#include "groups/arithmetic_group.h"
#include "groups/call_group.h"
#include "groups/data_group.h"
#include "groups/jump_group.h"
#include "groups/logical_group.h"
#include "groups/return_group.h"
#include "groups/special_group.h"
#include "groups/stack_group.h"

/* Lookup instructions for disassemble.
http://pastraiser.com/cpu/i8080/i8080_opcodes.html
*/
static const char * lookup[] =
	{
		"nop", "lxi b, +2", "stax b", "inx b",
		"inr b", "dcr b", "mvi b, +1", "rlc",
		"nop", "dad b", "ldax b", "dcx b",
		"inr c", "dcr c", "mvi c, +1", "rrc",
		"nop", "lxi d, +2", "stax d", "inx d",
		"inr d", "dcr d", "mvi d, +1", "ral",
		"nop", "dad d", "ldax d", "dcx d",
		"inr e", "dcr e", "mvi e, +1", "rar",
		"nop", "lxi h, +2", "shld", "inx h",
		"inr h", "dcr h", "mvi h, +1", "daa",
		"nop", "dad h", "lhld +2", "dcx h",
		"inr l", "dcr l", "mvi l, +1", "cma",
		"nop", "lxi sp, +2", "sta +2", "inx sp",
		"inr m", "dcr m", "mvi m, +1", "stc",
		"nop", "dad sp", "lda +2", "dcx sp",
		"inr a", "dcr a", "mvi a, +1", "cmc",
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
		"rnz", "pop b", "jnz +2", "jmp +2",
		"cnz +2", "push b", "adi +1", "rst 0",
		"rz", "ret", "jz +2", "jmp +2",
		"cz +2", "call +2", "aci +1", "rst 1",
		"rnc", "pop d", "jnc +2", "out +1",
		"cnc +2", "push d", "sui +1", "rst 2",
		"rc", "ret", "jc +2", "in +1",
		"cc +2", "call +2", "sbi +1", "rst 3",
		"rpo", "pop h", "jpo +2", "xthl",
		"cpo +2", "push h", "ani +1", "rst 4",
		"rpe", "pchl", "jpe +2", "xchg",
		"cpe +2", "call +2", "xri +1", "rst 5",
		"rp", "pop psw", "jp +2", "di",
		"cp +2", "push psw", "ori +1", "rst 6",
		"rm", "sphl", "jm +2", "ei",
		"cm +2", "call +2", "cpi +1", "rst 7"
	};

/* Instruction decode table. */
const Instruction decode[] =
	{
		&instr_nop, &instr_lxib, &instr_staxb, &instr_nop, /*0x03*/
		&instr_nop, &instr_nop, &instr_mvir, &instr_nop, /*0x07*/
		&instr_nop, &instr_dadb, &instr_ldaxb, &instr_nop, /*0x0B*/
		&instr_nop, &instr_nop, &instr_mvir, &instr_nop, /*0x0F*/
		&instr_nop, &instr_lxid, &instr_staxd, &instr_nop, /*0x13*/
		&instr_nop, &instr_nop, &instr_mvir, &instr_nop, /*0x17*/
		&instr_nop, &instr_dadd, &instr_ldaxd, &instr_nop, /*0x1B*/
		&instr_nop, &instr_nop, &instr_mvir, &instr_nop, /*0x1F*/
		&instr_nop, &instr_lxih, &instr_shld, &instr_nop, /*0x23*/
		&instr_nop, &instr_nop, &instr_mvir, &instr_nop, /*0x27*/
		&instr_nop, &instr_dadh, &instr_lhld, &instr_nop, /*0x2B*/
		&instr_nop, &instr_nop, &instr_mvir, &instr_cma, /*0x2F*/
		&instr_nop, &instr_lxisp, &instr_sta, &instr_inxsp, /*0x33*/
		&instr_nop, &instr_nop, &instr_mvim, &instr_stc, /*0x37*/
		&instr_nop, &instr_dads, &instr_lda, &instr_dcxsp, /*0x3B*/
		&instr_nop, &instr_nop, &instr_mvir, &instr_cmc, /*0x3F*/
		&instr_movrr, &instr_movrr, &instr_movrr, &instr_movrr, /*0x43*/
		&instr_movrr, &instr_movrr, &instr_movrm, &instr_movrr, /*0x47*/
		&instr_movrr, &instr_movrr, &instr_movrr, &instr_movrr, /*0x4B*/
		&instr_movrr, &instr_movrr, &instr_movrm, &instr_movrr, /*0x4F*/
		&instr_movrr, &instr_movrr, &instr_movrr, &instr_movrr, /*0x53*/
		&instr_movrr, &instr_movrr, &instr_movrm, &instr_movrr, /*0x57*/
		&instr_movrr, &instr_movrr, &instr_movrr, &instr_movrr, /*0x5B*/
		&instr_movrr, &instr_movrr, &instr_movrm, &instr_movrr, /*0x5F*/
		&instr_movrr, &instr_movrr, &instr_movrr, &instr_movrr, /*0x63*/
		&instr_movrr, &instr_movrr, &instr_movrm, &instr_movrr, /*0x67*/
		&instr_movrr, &instr_movrr, &instr_movrr, &instr_movrr, /*0x6B*/
		&instr_movrr, &instr_movrr, &instr_movrm, &instr_movrr, /*0x6F*/
		&instr_movmr, &instr_movmr, &instr_movmr, &instr_movmr, /*0x73*/
		&instr_movmr, &instr_movmr, &instr_hlt, &instr_movmr, /*0x77*/
		&instr_movrr, &instr_movrr, &instr_movrr, &instr_movrr, /*0x7B*/
		&instr_movrr, &instr_movrr, &instr_movrm, &instr_movrr, /*0x7F*/
		&instr_addr, &instr_addr, &instr_addr, &instr_addr, /*0x83*/
		&instr_addr, &instr_addr, &instr_addm, &instr_addr, /*0x87*/
		&instr_adcr, &instr_adcr, &instr_adcr, &instr_adcr, /*0x8B*/
		&instr_adcr, &instr_adcr, &instr_adcm, &instr_adcr, /*0x8F*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x93*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x97*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x9B*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x9F*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xA3*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xA7*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xAB*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xAF*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xB3*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xB7*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xBB*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xBF*/
		&instr_nop, &instr_popb, &instr_jnz, &instr_jmp, /*0xC3*/
		&instr_nop, &instr_pushb, &instr_adi, &instr_nop, /*0xC7*/
		&instr_nop, &instr_nop, &instr_jz, &instr_jmp, /*0xCB*/
		&instr_nop, &instr_nop, &instr_aci, &instr_nop, /*0xCF*/
		&instr_nop, &instr_popd, &instr_jnc, &instr_nop, /*0xD3*/
		&instr_nop, &instr_pushd, &instr_nop, &instr_nop, /*0xD7*/
		&instr_nop, &instr_nop, &instr_jc, &instr_nop, /*0xDB*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xDF*/
		&instr_nop, &instr_poph, &instr_jpo, &instr_xthl, /*0xE3*/
		&instr_nop, &instr_pushh, &instr_nop, &instr_nop, /*0xE7*/
		&instr_nop, &instr_pchl, &instr_jpe, &instr_xchg, /*0xEB*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xEF*/
		&instr_nop, &instr_popp, &instr_jp, &instr_nop, /*0xF3*/
		&instr_nop, &instr_pushp, &instr_nop, &instr_nop, /*0xF7*/
		&instr_nop, &instr_sphl, &instr_jm, &instr_nop, /*0xFB*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop  /*0xFF*/
	};

I8080_State * init_8080() {
	I8080_State * ret = malloc(sizeof(I8080_State));
	ret->pc = 0x0000;
	ret->flags = 0x02;
	ret->sp = 0xFFFF;
	ret->hlt = 0;
	return ret;
}

/* Generate PZS flags from Reg A */
void gen_pzs(I8080_State * s) {
	uint8_t count, bit, temp;
	/* Zero */
	COND_FLAG(!s->regs[REG_A], s, FLG_Z);
	/* Sign */
	COND_FLAG(s->regs[REG_A] & 0x80, s, FLG_S);
	/* Parity */
	temp = s->regs[REG_A];
	count = 0;
	for (bit = 0; bit < 8; bit++) {
		if (temp & 0x01) count++;
		temp >>= 1;
	}
	COND_FLAG(!(count % 2), s, FLG_P);
}

void disassemble_opcode(uint8_t opcode, I8080_State * s) {
	char * idx = strchr(lookup[opcode], '+'), * opstr;
	if (idx) {
		opstr = malloc((idx - lookup[opcode]) * sizeof(char));
		strncpy(opstr, lookup[opcode], idx - lookup[opcode]);
		/* Detect operand count */
		if (idx[1] == '1') {
			printf("OP: %s0x%02x \r\n", opstr, s->mem[s->pc + 1]);
		} else {
			/* N.B. LE */
			printf("OP: %s0x%02x%02x \r\n", opstr, s->mem[s->pc + 2], s->mem[s->pc + 1]);
		}
		free(opstr);
	} else { /* Opcode only,no operand */
		printf("OP: %s \r\n", lookup[opcode]);
	}
}

void dbg_8080(I8080_State * s) {
	puts("-------------------------------------------------");
	printf("PC: 0x%04x \t [PC]: 0x%02x \t", s->pc, s->mem[s->pc]);
	disassemble_opcode(s->mem[s->pc], s);
	printf("A:  0x%02x \t F: 0x%02x \r\n", s->regs[REG_A], s->flags);
	printf("B:  0x%02x \t C: 0x%02x \r\n", s->regs[REG_B], s->regs[REG_C]);
	printf("D:  0x%02x \t E: 0x%02x \r\n", s->regs[REG_D], s->regs[REG_E]);
	printf("H:  0x%02x \t L: 0x%02x \r\n", s->regs[REG_H], s->regs[REG_L]);
	printf("SP: 0x%04x \t [SP]: 0x%02x \t [SP+1]: 0x%02x \r\n", s->sp, s->mem[s->sp], s->mem[s->sp + 1]);
	printf("S: %x Z: %x \t A: %x P: %x \t C: %x \r\n", (s->flags & FLG_S) >> 7,
		(s->flags & FLG_Z) >> 6, (s->flags & FLG_A) >> 4, (s->flags & FLG_P) >> 2, (s->flags & FLG_C));
}

/* Run... */
void run_8080(I8080_State * s) {
	while (!s->hlt) {
		/* Execute current opcode */
		(*decode[s->mem[s->pc]])(s);
		dbg_8080(s);
	}
}
