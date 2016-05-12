#include <opcodes.h>

/* FUNC_TABLE functions are callable like (*FUNC_TABLE[0])(reg1, reg2)  for adding one register to another */
reg* (*FUNC_TABLE[FUNC_TABLE_SIZE])(int, int, int) = {add, mul, divide, sub, ld, sav, p, sto, set, p_str}; // Function table

int in_bounds(int i) {
  return i >= 0 && i < REGISTERTABLE_SIZE;
}

/*
  For a breakdown of every opcode function listed here, included example usage, side effects and known bugs, see the Documentation.org file.
 */

/*
  Print a string, as interpreted by a range of characters between a lower limit (register 1) and an upper limit (register 2).
 */
OPCODE_FUNC(p_str) {
  /* Check both the first and second arguments are in bounds */
  if (!in_bounds(car) || !in_bounds(cadr)) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    strcpy(err_reg->error, "-> ASM: Error: Index Out Of Bounds");
    return err_reg;
  }
  reg* r1 = REGISTERTABLE[car];
  reg* r2 = REGISTERTABLE[cadr];
  int size = r2->val - r1->val;
  /* Check the user hasn't got an invalid string range */
  if (size <= 0) {
    err_reg->err = INVALID_STR_LENGTH_ERROR;
    strcpy(err_reg->error, "-> ASM: Error: Invalid String Length");
    return err_reg;
  }
  char string[size + 1];
  memset(string, 0, size + 1);
  /* For each register in between the value of register 1 and the value of register 2 */
  for (int i = r1->val; i < r2->val; i++) {
    /* Add to the string the character value at register one */
    reg* r = MEMTABLE[i];
    string[i] = (char)r->val;
  }
  string[size + 1] = '\0';
  /* We use the io register instead of an actual register here because modifying the other registers could result in unexpected behaviours we can't for see. */
  sprintf(io_reg->debug, "-> %s", string);
  return io_reg;
}
/* Set the value of a register */
OPCODE_FUNC(set) {
  /* Check the first argument is in bounds */
  if (!in_bounds(car)) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    strcpy(err_reg->error, "-> ASM: Error: Index Out Of Bounds");
    return err_reg;
  }
  reg* to = REGISTERTABLE[car];
  /*
    Set the value as the second plus the third arguments.

    This looks like this in the interpreter

    1000 0000             1111                                  1111
         ^^^^             ^^^^                                  ^^^^
         register to set. second argument (interpreted as 240). Third argument (interpreted as 15)
    So the value to set becomes 240 + 15 (255).
   */
  to->val = cadr + caddr;
  sprintf(to->debug, "-> %i", to->val);
  return to;
}
/* Add two registers values together */
OPCODE_FUNC(add) {
  /* Check both specified registers are in bounds */
  if(!in_bounds(car) || !in_bounds(cadr)) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    strcpy(err_reg->error, "-> ASM: Error: Index Out Of Bounds");
    return err_reg;
  }
  reg* to = REGISTERTABLE[car];
  reg* from = REGISTERTABLE[cadr];
  /* add the value of the second register to the first register, storing the value in the first register. */
  to->val += from->val;
  sprintf(to->debug, "-> %i", to->val);
  return to;
}
/* Subtract the value of two registers for ellipses see the add function */
OPCODE_FUNC(sub) {
  /* ... */
  if(!in_bounds(car) || !in_bounds(cadr)) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    strcpy(err_reg->error, "-> ASM: Error: Index Out Of Bounds");
    return err_reg;
  }
  reg* to = REGISTERTABLE[car];
  reg* from = REGISTERTABLE[cadr];
  /* Subtract the value of ... */
  to->val -= from->val;
  sprintf(to->debug, "-> %i", to->val);
  return to;
}
/* Multiply the value of two registers */
OPCODE_FUNC(mul) {
  /* ... */
  if(!in_bounds(car) || !in_bounds(cadr)) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    strcpy(err_reg->error, "-> ASM: Error: Index Out Of Bounds");
    return err_reg;
  }
  reg* to = REGISTERTABLE[car];
  reg* from = REGISTERTABLE[cadr];
  /* Multiply the value of two registers ... */
  to->val *= from->val;
  sprintf(to->debug, "-> %i", to->val);
  return to;
}
/* Divide the value of one register by another */
OPCODE_FUNC(divide) {
  /* ... */
  if(!in_bounds(car) || !in_bounds(cadr)) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    strcpy(err_reg->error, "-> ASM: Error: Index Out Of Bounds");
    return err_reg;
  }
  reg* to = REGISTERTABLE[car];
  reg* from = REGISTERTABLE[cadr];
  /* Special divide by zero check :) */
  if (from->val == 0) {
    err_reg->err = DIV_BY_ZERO_ERROR;
    strcpy(err_reg->error, "-> ASM: Error: Cannot Divide By Zero");
    return err_reg;
  }
  to->val /= from->val;
  sprintf(to->debug, "-> %i", to->val);
  return to;
}
/* Load the value of the memory location specified by the second register to the location of the first */
OPCODE_FUNC(ld) {
  /* Check if the value is in the memory table, and in the register table */
  if(!in_bounds(car) || cadr > MEMTABLE_SIZE || cadr < 0) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    strcpy(err_reg->error, "-> ASM: Error: Index Out Of Bounds");
    return err_reg;
  }
  reg* to = REGISTERTABLE[car];
  /* memmove is destructive (as opposed to memcpy), and therefore the value of a register is not left behind when you move it */
  to = memmove(to, MEMTABLE[cadr], sizeof(reg));
  sprintf(to->debug, "-> SUCCESS");
  return to;
}
/* Save the value of a register in a memory location given by register two */
OPCODE_FUNC(sav) {
  /* ... */
  if(!in_bounds(car) || cadr > MEMTABLE_SIZE || cadr < 0) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    strcpy(err_reg->error, "-> ASM: Error: Index Out Of Bounds");
    return err_reg;
  }
  reg* from = REGISTERTABLE[car];
  reg* to = MEMTABLE[cadr];
  /* Non-destructively copy the from the value of register and save it to the memory location */
  memcpy(to, from, sizeof(reg));
  sprintf(from->debug, "-> SUCCESS");
  return from;
}
/* Print the value of a register */
OPCODE_FUNC(p) {
  /* Check the register is in bounds */
  if(!in_bounds(car)) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    strcpy(err_reg->error, "-> ASM: Error: Index Out Of Bounds");
    return err_reg;
  }
  reg* regi = REGISTERTABLE[car];
  sprintf(regi->debug, "-> %i", regi->val);
  return regi;
}
/* Store the value of the register to the memory location of the second arguments, destructively */
OPCODE_FUNC(sto) {
  if(!in_bounds(car) || cadr > MEMTABLE_SIZE || cadr < 0) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    strcpy(err_reg->error, "-> ASM: Error: Index Out Of Bounds");
    return err_reg;
  }
  reg* from = REGISTERTABLE[car];
  reg* to = MEMTABLE[cadr];
  memcpy(to, from, sizeof(reg));
  /* Write over the register */
  memset(from, 0, sizeof(reg));
  sprintf(from->debug, "-> SUCCESS");
  from->val = 0;
  return from;
}
/* Create a register, allocated the correct heap memory required */
reg* new_register() {
  reg* ptr = malloc(sizeof(reg));
  ptr->val = 0;
  strcpy(ptr->debug, "");
  strcpy(ptr->error, "");
  ptr->err = 0;
  return ptr;
}
/* Initialize the interpreter, and fill the memory/register tables with registers manipulate */
void init_asm_interpreter() {
  err_reg = new_register();
  io_reg = new_register();
  for (int i = 0; i < REGISTERTABLE_SIZE; i++) {
    REGISTERTABLE[i] = new_register();
  }
  for (int i = 0; i < MEMTABLE_SIZE; i++) {
    MEMTABLE[i] = new_register();
  }
}
void free_asm_interpreter() {
  free(err_reg);
  free(io_reg);
  for (int i = 0; i < REGISTERTABLE_SIZE; i++) {
    free(REGISTERTABLE[i]);
  }
  for (int i = 0; i < MEMTABLE_SIZE; i++) {
    free(MEMTABLE[i]);
  }
}
/* Convert any character literally to an integer ('0' -> 0) */
int ctoi(char c) {
  if ('0' <= c &&  c <= '9') {
    return c - '0';
  } else {
    /* character was not a number */
    return CTOI_ERR;
  }
}
/* Parse a binary string into a number */
int parse_binary(char* binary, int factor) {
  int power = strlen(binary) - 1;
  int ret = 0;
  for (char *p = binary; *p != '\0'; p++) {
    int current_number = ctoi(*p);
    if (current_number == CTOI_ERR) {
      return BINARY_ERR;
    }
    else if (current_number) {
      ret += pow(2, (power + factor));
    }
    power--;
  }
  return ret;
}
/* Parse and run a given set of strings (this is the main interface to the interpreter), and in turn return the register result of the opcode */
reg* parse_and_run(char* func, char* car, char* cadr, char* caddr) {
  int function, c, cd, cdd;
  function = parse_binary(func, 0);
  if (function > FUNC_TABLE_SIZE) {
    err_reg->err = UNDEFINED_FUNCTION;
    strcpy(err_reg->error, "-> ASM: Error: Invalid Function Call");
    return err_reg;
  }
  c = parse_binary(car, 0);
  /* If the function  is set, then we need to parse the second and third numbers too. */
  cd = parse_binary(cadr, function == 8 ? 4 : 0);
  cdd = parse_binary(caddr, 0);
  if (function == BINARY_ERR ||
      c == BINARY_ERR ||
      cd == BINARY_ERR ||
      cdd == BINARY_ERR) {
    err_reg->err = BINARY_ERR;
    strcpy(err_reg->error, "-> ASM: Error: Invalid Binary Input");
    return err_reg;
  }
  /* This piece of voodo roughly translates to run the function (index function) in the array  with the arguments c, cd, and cdd */
  return (*FUNC_TABLE[function])(c, cd, cdd);
}
