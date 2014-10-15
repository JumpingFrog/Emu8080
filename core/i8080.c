#include "i8080.h"
#include "stdio.h"
#include "stdlib.h"
/* Instruction groups */
#include "groups/arithmetic_group.h"
#include "groups/call_group.h"
#include "groups/data_group.h"
#include "groups/jump_group.h"
#include "groups/logical_group.h"
#include "groups/return_group.h"
#include "groups/special_group.h"
#include "groups/stack_group.h"

/* Instruction decode table 
http://pastraiser.com/cpu/i8080/i8080_opcodes.html
*/

const Instruction decode[] = 
	{
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x03*/
		&instr_nop, &instr_nop, &instr_mvir, &instr_nop, /*0x07*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x0B*/
		&instr_nop, &instr_nop, &instr_mvir, &instr_nop, /*0x0F*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x13*/
		&instr_nop, &instr_nop, &instr_mvir, &instr_nop, /*0x17*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x1B*/
		&instr_nop, &instr_nop, &instr_mvir, &instr_nop, /*0x1F*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x23*/
		&instr_nop, &instr_nop, &instr_mvir, &instr_nop, /*0x27*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x2B*/
		&instr_nop, &instr_nop, &instr_mvir, &instr_nop, /*0x2F*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x33*/
		&instr_nop, &instr_nop, &instr_mvim, &instr_nop, /*0x37*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x3B*/
		&instr_nop, &instr_nop, &instr_mvir, &instr_nop, /*0x3F*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x43*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x47*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x4B*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x4F*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x53*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x57*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x5B*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x5F*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x63*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x67*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x6B*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x6F*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x73*/
		&instr_nop, &instr_nop, &instr_hlt, &instr_nop, /*0x77*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x7B*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x7F*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x83*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x87*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x8B*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x8F*/
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
		&instr_nop, &instr_nop, &instr_nop, &instr_jmp, /*0xC3*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xC7*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xCB*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xCF*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xD3*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xD7*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xDB*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xDF*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xE3*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xE7*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xEB*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xEF*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xF3*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xF7*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0xFB*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop  /*0xFF*/
	};

I8080_State * init_8080() {
	I8080_State * ret = malloc(sizeof(I8080_State));
	ret->pc = 0x0000;
	ret->flags = 0x02;
	ret->sp = 0xFFFF;
	ret->hlt = 0;
	
	ret->mem[0xFFF0] = 0xC3;
	ret->mem[0xFFF1] = 0xA0;
	ret->mem[0xFFF2] = 0xFF;
	ret->mem[0xFFFD] = 0x76;
	return ret;
}

void gen_flags(I8080_State * s) {
	/* ... */
}

void dbg(I8080_State * s) {
	printf("PC 0x%x\n\r", s->pc);
}

/* Run... */
void run(I8080_State * s) {
	while (!s->hlt) {
		/* Execute current opcode */
		(*decode[s->mem[s->pc]])(s);
		dbg(s);
	}	
}

int main(int argc, char * argv[]) {
	I8080_State * r = init_8080();
	printf("Decode: %lu instructions.\n\r", (sizeof(decode)/sizeof(Instruction)));
	printf("SP: 0x%x.\n\r", r->sp);
	run(r);
	return 0;
}