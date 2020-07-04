#include <stdio.h>
#include <stdlib.h>
#include <i8080.h>
#include <devices/serial.h>

int main(int argc, char * argv[]) {
	FILE *fp;
	I8080State *r = init_8080();
	IODevice *serial = init_serial();
	/* Add serial device to IO port 0x02. */
	add_dev_8080(r, 0x02, serial);

	/* Init stuff */
	if(argc != 2) {
		puts("Usage: emu <rom_file>");
	}
	else if ((fp = fopen(argv[1], "rb"))) {
		printf("Read in %lu bytes from rom.bin\r\n", fread(r->mem, sizeof(uint8_t), 0xFFFF, fp));
		fclose(fp);
		/* Go */
		run_8080(r);
	}
	else {
		printf("No file, %s, Exiting\n", argv[1]);
	}

	free(r);

	return 0;
}

