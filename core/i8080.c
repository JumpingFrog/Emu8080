#include "i8080.h"
#include "stdio.h"
#include "stdlib.h"

void instr_nop(I8080_State * s) {
	s->pc++;
}

/* Instruction decode table */

const Instruction decode[] = 
	{
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
	return ret;
}

/* Run... */
void run(I8080_State * s) {
	uint8_t hlt = 0;
	while (!hlt) {
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