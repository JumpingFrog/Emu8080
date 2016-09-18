#include "stdint.h"

/* Macros for Register functionality */
#define REG_A 7
#define REG_B 0
#define REG_C 1
#define REG_D 2
#define REG_E 3
#define REG_H 4
#define REG_L 5
#define REG_M 6

#define REG(S, X) (X==REG_M) ? (S->regs[REG_H] << 8) | S->regs[REG_L] : S->regs[X];

/* Macros for flag functionality */
#define FLG_C 0x01
#define FLG_P 0x04
#define FLG_A 0x10
#define FLG_Z 0x40
#define FLG_S 0x80

#define FLAG(S, X) ((S->flags & X) ? 1 : 0)

/* Macros for register pairs */
#define RP_BC(S) ((S->regs[REG_B] << 8) | S->regs[REG_C])
#define RP_DE(S) ((S->regs[REG_D] << 8) | S->regs[REG_E])
#define RP_HL(S) ((S->regs[REG_H] << 8) | S->regs[REG_L])

/* Macro to set a flag based on condition. Cleared if cond is false. */
#define COND_FLAG(COND, S, F) if (COND) (S)->flags |= F; else (S)->flags &= ~F;
#define GEN_CY(V, S) COND_FLAG((V) & 0x100, (S), FLG_C);
#define GEN_AC(A, B, S) COND_FLAG(((A) & 0X0F) + ((B) & 0x0F) & 0x10, (S), FLG_A);

/* Debug print macro */
#define DBG_PRINT
#ifdef DBG_PRINT
	#define DBG(...) printf(__VA_ARGS__)
#else
	#define DBG(...)
#endif

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
	uint16_t sp;
	/* Memory */
	uint8_t mem[0xFFFF];
	/* IO Devices */
	struct _IODevice *devices[256];
} I8080_State;

/* Typedefs for IO device functionality.
	Consider if passing state is neccesary...
 */
typedef uint8_t (*IO_in)(I8080_State *);
typedef void (*IO_out)(I8080_State *);

/* Typedef fdr IO Devices, implement in and out methods. */
typedef struct _IODevice {
	IO_in in;
	IO_out out;
} IODevice;

/* Typedef for instructions */
typedef void (*Instruction)(I8080_State *);

/* Prototypes */
void run_8080(I8080_State *);
I8080_State *init_8080();
void gen_pzs(I8080_State *);
void add_dev_8080(I8080_State *, uint8_t, IODevice *);
