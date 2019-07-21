/* SPECIAL, I/O, CONTROL */

void instr_cma(I8080State *);
void instr_stc(I8080State *);
void instr_cmc(I8080State *);
void instr_daa(I8080State *);

void instr_in(I8080State *);
void instr_out(I8080State *);

void instr_ei(I8080State *);
void instr_di(I8080State *);
void instr_nop(I8080State *);
void instr_hlt(I8080State *);

