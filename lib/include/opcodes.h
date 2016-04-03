#ifndef OPCODES_H
#define OPCODES_H
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef enum {ADD, MUL, DIV, SUB, LD, SAV, P, STO, SET} OPCODE;
// Memory table, is just a list of integers
// Register struct, a wrap around an int with a debug message for the results of the last instruction
#define OPCODE_FUNC(NAME) reg* NAME(int car, int cadr, int caddr)
#define MEMTABLE_SIZE 15
#define REGISTERTABLE_SIZE 2
#define FUNC_TABLE_SIZE 10
#define BINARY_ERR -1
#define CTOI_ERR -2
#define OUT_OF_BOUNDS_ERROR -3
#define DIV_BY_ZERO_ERROR -4
#define UNDEFINED_FUNCTION -5
#define INVALID_STR_LENGTH_ERROR -6
typedef struct reg {
  uint8_t val;
  char debug[255]; // for printing the interpreted results
  int err;
  char error[64];
}reg;
reg* MEMTABLE[MEMTABLE_SIZE];
reg* REGISTERTABLE[REGISTERTABLE_SIZE]; // the two registers
reg* err_reg;
reg* io_reg;
OPCODE_FUNC(add);
OPCODE_FUNC(mul);
OPCODE_FUNC(divide);
OPCODE_FUNC(sub);
OPCODE_FUNC(ld);
OPCODE_FUNC(sav);
OPCODE_FUNC(p);
OPCODE_FUNC(sto);
OPCODE_FUNC(set);
OPCODE_FUNC(p_str);
int in_bounds(int i);
reg* new_register();
void init_asm_interpreter();
void free_asm_interpreter();
int ctoi(char c);
int parse_binary(char* binary, int factor);
reg* parse_and_run(char* func, char* car, char* cadr, char* caddr);
void reverse_string(char * str);
/* FUNC_TABLE functions are callable like (*FUNC_TABLE[0])(reg1, reg2)  for adding one register to another */
#endif /* OPCODES_H */
