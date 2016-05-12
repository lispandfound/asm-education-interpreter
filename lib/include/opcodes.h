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
  /* err is a flag that tells the CLI whether or not an error occurred */
  int err;
  /* the actual error string buffer */
  char error[64];
}reg;
/* This is the memory table, it needs to be globally available as it is used across all functions */
reg* MEMTABLE[MEMTABLE_SIZE];
/* This is the register table, it needs to be globally available as it is used across all functions */
reg* REGISTERTABLE[REGISTERTABLE_SIZE]; // the two registers
/*
  Because C does not have generics or exceptions, I do not have the ability for multiple return types of a function or indeed the ability to throw an error.
  Therefore a register will need to be returned from the opcodes regardless of whether it succeeded or not. I am not really comfortable editing the registers in the register table,
  these registers are for results of successful opcodes, rather than errors or io output (in p_str's case). As such I have declared two special registers, whose sole job is for error and io respectively.
  They are not returned in any case bar error's or io output, and therefore I avoid accidentally editing the real registers.
 */
reg* err_reg;
reg* io_reg;

/* Opcode functions. These functions actually perform mutations on the registers. The actual function signatures can be seen at OPCODE_FUNC */
/* Add two registers */
OPCODE_FUNC(add);
/* Multiple two registers */
OPCODE_FUNC(mul);
/* Divide two registers (div was already taken) */
OPCODE_FUNC(divide);
/* Subtract two registers */
OPCODE_FUNC(sub);
/* Load register from memory location (destructive!) */
OPCODE_FUNC(ld);
/* Save register to memory location (non destructive) */
OPCODE_FUNC(sav);
/* Print the value of a register (not to be confused with p_str, which prints a string) */
OPCODE_FUNC(p);
/* Store a register in memory (destructive!) */
OPCODE_FUNC(sto);
/* Set the value of a register */
OPCODE_FUNC(set);
/* Print a range of registers in memory, interpreting each register's as an ascii character (e.g 98 -> 'a') */
OPCODE_FUNC(p_str);
/* Check if an index is in bounds or the register table */
int in_bounds(int i);
/* Create a new register (allocates memory on the heap for it) */
reg* new_register();
/* Initialize and free the interpreter */
void init_asm_interpreter();
void free_asm_interpreter();
/* Convert a character to an integer (literally '0' -> 0) */
int ctoi(char c);
/* Convert binary to decimal, accounting for a factor if a half byte is already taken */
int parse_binary(char* binary, int factor);
/* parse and run the given sections returning the results of the corresponding opcodes running back */
reg* parse_and_run(char* func, char* car, char* cadr, char* caddr);
void reverse_string(char * str);
#endif /* OPCODES_H */
