#include <i8080.h>
#include <logical_group.h>

/* Register AND */
void instr_anar(I8080_State * s) {
	uint8_t r = s->mem[s->pc] & 0x03;
	s->regs[REG_A] &= s->regs[r];
	/* RELOAD FLAGS */
}

/* Memory AND */
void instr_anam(I8080_State * s) {

}
