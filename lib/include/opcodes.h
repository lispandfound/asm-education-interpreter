#ifndef OPCODES_H
#define OPCODES_H
#include <stdio.h>

#define OPCODE_FUNC(NAME) reg NAME(int car, int cadr, int caddr)

typedef struct reg {
  int val;
  char* debug; // for printing the interpreted results
}reg;

OPCODE_FUNC(add);
OPCODE_FUNC(mul);
OPCODE_FUNC(div);
OPCODE_FUNC(sub);
OPCODE_FUNC(ld);
OPCODE_FUNC(sav);
OPCODE_FUNC(p);
OPCODE_FUNC(sto);
reg (*FUNC_TABLE[8])(int, int, int) = {add, mul, div, sub, ld, sav, p, sto}; // Function table
/* FUNC_TABLE functions are callable like (*FUNC_TABLE[0])(reg1, reg2)  for adding one register to another */
#endif /* OPCODES_H */
