#include "stdio.h"
#include "core/i8080.h"

int main(int argc, char * argv[]) {
	FILE * fp;
	I8080_State * r = init_8080();
	/* Init stuff */
	fp = fopen("test.bin", "rb");
	printf("Read in %lu bytes from test.bin\n\r", fread(r->mem, sizeof(uint8_t), 65536, fp));
	fclose(fp);

	/* Go */
	run_8080(r);
	return 0;
}