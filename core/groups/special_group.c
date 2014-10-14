#include "../i8080.h"
#include "special_group.h"

void instr_nop(I8080_State * s) {
	s->pc++;
}