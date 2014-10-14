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

/* Instruction decode table */

const Instruction decode[] = 
	{
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x03*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x07*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x0A*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x0E*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x13*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x17*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x1A*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x1E*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x23*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x27*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x2A*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x2E*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x33*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x37*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x3A*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x3E*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x43*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x47*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x4A*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x4E*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x53*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x57*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x5A*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x5E*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x63*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x67*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x6A*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x6E*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x73*/
		&instr_nop, &instr_nop, &instr_hlt, &instr_nop, /*0x77*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x7A*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x83*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x87*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop, /*0x8A*/
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop,
		&instr_nop, &instr_nop, &instr_nop, &instr_nop
	};

I8080_State * init_8080() {
	I8080_State * ret = ret = malloc(sizeof(I8080_State));
	ret->pc = 0x0000;
	ret->flags = 0x02;
	ret->sp = 0xFFFF;
	ret->hlt = 0;

	ret->mem[0xFFFD] = 0x76;
	return ret;
}

/* Run... */
void run(I8080_State * s) {
	while (!s->hlt) {
		/* Execute current opcode */
		(*decode[s->mem[s->pc]])(s);
		printf("PC value: %d\n\r", s->pc);
	}	
}

int main(int argc, char * argv[]) {
	I8080_State * r = init_8080();
	printf("Decode: %lu instructions.\n\r", (sizeof(decode)/sizeof(Instruction)));
	printf("SP: 0x%x.\n\r", r->sp);
	run(r);
	return 0;
}