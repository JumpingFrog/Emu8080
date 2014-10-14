#include "stdint.h"

#define A 7
#define B 0
#define C 1
#define D 2
#define E 3
#define H 4
#define L 5
#define M 6

/* Data for 8080 */
typedef struct {
	/* Halt flag */
	uint8_t hlt;
	/* Registers */
	uint8_t regs[8];
	/* Program Counter */
	uint16_t pc;
	/* Flags Register */
	uint8_t flags;
	/* Stack Pointer */
	uint16_t sp ;
	/* Memory */
	uint8_t mem[0xFFFF];
	/* IO Devices */
	struct IODevice * devices[256];
} I8080_State; 


/* Typedefs for IO device functionality.
	Consider if passing state is neccesary...
 */
typedef uint8_t (*IO_in)(I8080_State *);
typedef void (*IO_out)(I8080_State *);

/* Typedef fdr IO Devices, implement in and out methods. */
typedef struct {
	IO_in in;
	IO_out out;
} IODevice;

/* Typedef for instructions */
typedef void (*Instruction)(I8080_State *);