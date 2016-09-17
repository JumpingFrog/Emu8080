#include <stdio.h>
#include <stdlib.h>
#include <i8080.h>
#include <devices/serial.h>

int main(int argc, char * argv[]) {
	FILE *fp;
	I8080_State *r = init_8080();
	IODevice *serial = init_serial();
	/* Add serial device to IO port 0x02. */
	add_dev_8080(r, 0x02, serial);

	/* Init stuff */
	if ((fp = fopen("rom.bin", "rb"))) {
		printf("Read in %lu bytes from rom.bin\r\n", fread(r->mem, sizeof(uint8_t), 0xFFFF, fp));
		fclose(fp);
		/* Go */
		run_8080(r);
	}
	else {
		puts("No file; rom.bin. Exiting.");
	}

	free(r);

	return 0;
}

