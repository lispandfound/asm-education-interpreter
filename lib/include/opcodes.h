#ifndef OPCODES_H
#define OPCODES_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
typedef enum {ADD, MUL, DIV, SUB, LD, SAV, P, STO} OPCODE;
// Memory table, is just a list of integers
// Register struct, a wrap around an int with a debug message for the results of the last instruction
#define OPCODE_FUNC(NAME) reg* NAME(int car, int cadr, int caddr)
#define MEMTABLE_SIZE 64
#define REGISTERTABLE_SIZE 2
typedef struct reg {
  int val;
  char debug[64]; // for printing the interpreted results
}reg;
reg* MEMTABLE[MEMTABLE_SIZE];
reg* REGISTERTABLE[REGISTERTABLE_SIZE]; // the two registers
OPCODE_FUNC(add);
OPCODE_FUNC(mul);
OPCODE_FUNC(divide);
OPCODE_FUNC(sub);
OPCODE_FUNC(ld);
OPCODE_FUNC(sav);
OPCODE_FUNC(p);
OPCODE_FUNC(sto);
reg* new_register();
void init_asm_interpreter();
void free_asm_interpreter();
/* FUNC_TABLE functions are callable like (*FUNC_TABLE[0])(reg1, reg2)  for adding one register to another */
#endif /* OPCODES_H */
