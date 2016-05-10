/*

  Opcodes.h

  This file is the header for opcodes.c, but also contains important macros that define error codes, register and memory table sizes

 */
#ifndef OPCODES_H
#define OPCODES_H
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* OPCODE_FUNC is the template for an opcode (e.g the Add opcode). The reason we use a macro here is to enhance readability, and to provide the an easy way to define consistent functions doing the same task */
#define OPCODE_FUNC(NAME) reg* NAME(int car, int cadr, int caddr)
#define MEMTABLE_SIZE 15
#define REGISTERTABLE_SIZE 2
#define FUNC_TABLE_SIZE 10
#define BINARY_ERR -1
/* CTOI is short for character to integer */
#define CTOI_ERR -2
#define OUT_OF_BOUNDS_ERROR -3
#define DIV_BY_ZERO_ERROR -4
#define UNDEFINED_FUNCTION -5
#define INVALID_STR_LENGTH_ERROR -6
/* reg struct, this is the structure for registers, and is a critical part of the system in general */
typedef struct reg {
  /* An 8-bit integer storing the value of the register. The reason this is 8 bit is because that is the largest number about to be set by the SET opcode */
  uint8_t val;
  /* Debug contains the variable for debug information (so the return result of an OPCODE, e.g ADD adding 1 + 1 together might return "-> 2") */
  char debug[255];
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
