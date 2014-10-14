#include "i8080.h"
#include "stdio.h"
#include "stdlib.h"

void instr_nop(I8080_State * s) {
	s->pc++;
}

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
	I8080_State * ret = malloc(sizeof(I8080_State));
	ret->pc = 0x0000;
	ret->flags = 0x02;
	ret->sp = 0xFFFF;
	return ret;
}


int main(int argc, char * argv[]) {
	printf("Decode: %lu instructions.\n\r", (sizeof(decode)/sizeof(Instruction)));
	I8080_State * r = init_8080();
	printf("SP: 0x%x.\n\r", r->sp);
}