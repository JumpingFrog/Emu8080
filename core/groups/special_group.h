/* SPECIAL, I/O, CONTROL, RESTART */

Instruction instr_rst;

Instruction instr_cma;
Instruction instr_stc;
Instruction instr_cmc;
Instruction instr_daa;

Instruction instr_in;
Instruction instr_out;

Instruction instr_ei;
Instruction instr_di;
void instr_nop(I8080_State *);
void instr_hlt(I8080_State *);