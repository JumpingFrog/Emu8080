#include <stdint.h>
#include <stdio.h>

/* Modification flags */
#define MOD_REG(X)  (1 << X)
#define MOD_FLAGS   (1 << 8)
#define MOD_SP      (1 << 9)
#define MOD_PC      (1 << 10)

/* Macros for Register functionality */
#define REG_A 7
#define REG_B 0
#define REG_C 1
#define REG_D 2
#define REG_E 3
#define REG_H 4
#define REG_L 5
#define REG_M 6

#define READ_REG(S, X) (X==REG_M) ? ((S)->regs[REG_H] << 8) | (S)->regs[REG_L] : (S)->regs[X]

#define WRITE_REG(S, X, V)  if (X==REG_M) {\
                                (S)->regs[REG_H] = (V) >> 8;\
                                (S)->regs[REG_L] = (V) & 0xff;\
                            } \
                            else {\
                                (S)->regs[X] = (V) & 0xff;\
                            }\
                            (S)->reg_mod |= MOD_REG(X)

#define WRITE_SP(S, V) (S)->sp = (V); (S)->reg_mod |= MOD_SP
#define WRITE_PC(S, V) (S)->pc = (V); (S)->reg_mod |= MOD_PC

/* Macros for reading register pairs */
#define RP_BC(S) (((S)->regs[REG_B] << 8) | (S)->regs[REG_C])
#define RP_DE(S) (((S)->regs[REG_D] << 8) | (S)->regs[REG_E])
#define RP_HL(S) (((S)->regs[REG_H] << 8) | (S)->regs[REG_L])

/* Macros for flag functionality */
#define FLG_C 0x01
#define FLG_P 0x04
#define FLG_A 0x10
#define FLG_Z 0x40
#define FLG_S 0x80

#define READ_FLAG(S, X) (((S)->flags & X) ? 1 : 0)

/* Write entire flags register, maintaining RES0 and RES1 fields. */
#define WRITE_FLAGS(S, V)   (S)->flags = V | 0x2;\
                            (S)->flags &= 0xd7;\
                            (S)->reg_mod |= MOD_FLAGS

/* Macro to set a flag based on condition. Cleared if cond is false. */
#define COND_FLAG(S, COND, F)   if ((COND)) {\
                                    (S)->flags |= F;\
                                }\
                                else {\
                                    (S)->flags &= ~F;\
                                }\
                                (S)->reg_mod |= MOD_FLAGS

#define GEN_CY(S, V) COND_FLAG(S, (V) & 0x100, FLG_C)
/* Generate borrow */
#define GEN_CY_SUB(S, V) COND_FLAG(s, !((V) & 0x100), FLG_C)
#define GEN_AC(S, A, B) COND_FLAG(S, ((A) & 0x0F) + ((B) & 0x0F) & 0x10, FLG_A)
#define GEN_Z(S, V) COND_FLAG(S, !(V), FLG_Z)
#define GEN_S(S, V) COND_FLAG(S, (V) & 0x80, FLG_S)
#define GEN_PZS(S, V) GEN_Z(S, V); GEN_S(S, V); gen_p(S, V)

/* Macros for memory */
#define READ_MEM(S, A) ((S)->mem[A])
#define WRITE_MEM(S, A, V) ((S)->mem[A] = ((V) & 0xff))
#define READ_OP(S) READ_MEM(S, (S)->pc)
#define READ_IMM8(S) READ_MEM(S, (S)->pc + 1)
#define READ_IMM16(S) ((READ_MEM(S, (S)->pc + 2) << 8) | READ_MEM(S, (S)->pc + 1))

/* Debug print macro */
#define DBG_PRINT
#define TRACE_FILE

#ifdef TRACE_FILE
	#define TRACE(S, V) fputs(V, S->ftrace)
	#define TRACEF(S, ...) fprintf(S->ftrace, __VA_ARGS__)
#else
	#define TRACE(S, V) puts(V)
	#define TRACEF(S, ...) printf(__VA_ARGS__)
#endif

#ifdef DBG_PRINT
	#define DBG(S, ...) TRACEF(S, __VA_ARGS__)
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
	/* Modification vector, used for trace */
	/* |PC (10|SP (9)|flags (8)|REG_A (7)|REG_M (6)|REG_L (5)|REG_H (4)|REG_E (3)|REG_D (2)|REG_C (1)|REG_B (0)| */
	uint16_t reg_mod;
	/* IO Devices */
	struct _IODevice *devices[256];
	#ifdef TRACE_FILE
		FILE *ftrace;
	#endif
} I8080State;

/* Typedefs for IO device functionality.
	Consider if passing state is neccesary...
 */
typedef uint8_t (*IO_in)(I8080State *);
typedef void (*IO_out)(I8080State *);

/* Typedef for IO Devices, implement in and out methods. */
typedef struct _IODevice {
	IO_in in;
	IO_out out;
} IODevice;

typedef struct _MemoryChange {
	uint16_t addr;
	uint8_t value;
} MemoryChange;

typedef struct _MemoryChanges {
	uint8_t count;
	struct _MemoryChange changes[2];
} MemoryChanges;

/* Typedef for instructions */
typedef void (*Instruction)(I8080State *);

/* Prototypes */
void run_8080(I8080State *);
I8080State *init_8080();
void gen_p(I8080State *, uint8_t);
void add_dev_8080(I8080State *, uint8_t, IODevice *);
