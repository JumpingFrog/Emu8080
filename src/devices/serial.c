#include <i8080.h>
#include <devices/serial.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t in_serial(I8080State *s) {
	return getchar();
}

void out_serial(I8080State *s) {
	putchar(s->regs[REG_A]);
	fflush(0);
}

IODevice *init_serial(void) {
	/* TODO: Settings here */
	IODevice *result = malloc(sizeof(IODevice));
	result->in = in_serial;
	result->out = out_serial;
	return result;
}
