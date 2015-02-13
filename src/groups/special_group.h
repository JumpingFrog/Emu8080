/* SPECIAL, I/O, CONTROL, RESTART */

void instr_rst(I8080_State *);

void instr_cma(I8080_State *);
void instr_stc(I8080_State *);
void instr_cmc(I8080_State *);
void instr_daa(I8080_State *);

void instr_in(I8080_State *);
void instr_out(I8080_State *);

void instr_ei(I8080_State *);
void instr_di(I8080_State *);
void instr_nop(I8080_State *);
void instr_hlt(I8080_State *);
