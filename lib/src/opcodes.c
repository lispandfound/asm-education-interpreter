#include "opcodes.h"
// Op codes for DSL
typedef enum {ADD, MUL, DIV, SUB, LD, SAV, P, STO} OPCODE;
// Memory table, is just a list of integers
int MEMTABLE[64];
// Register struct, a wrap around an int with a debug message for the results of the last instruction
typedef struct reg {
  int val;
  char* debug; // for printing the interpreted results
}reg;
reg REGISTERTABLE[2]; // the two registers
// forgive my lisp
// this macro defines an opcode function for consistency reasons
OPCODE_FUNC(add) {
  printf("%i", car);
}
const OPCODE_FUNC_PTR(FUNC_TABLE) = {add, mul, div, sub, ld, sav, p, sto};
// TODO maybe function pointer table instead of these opcodes
