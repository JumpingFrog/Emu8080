#include "stdint.h"


typedef struct {
	/* Registers */
	uint8_t regs[7];
	/* Program Counter */
	uint16_t pc;
	/* Flags Register */
	uint8_t flags;
	/* Stack Pointer */
	uint16_t sp ;
	/* Memory */
	uint8_t mem[0xFFFF];
	/* IO Devices */
} I8080_State; 

/* Typedef for instructions */
typedef void (*Instruction)(I8080_State *);