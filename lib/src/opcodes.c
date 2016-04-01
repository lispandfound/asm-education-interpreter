#include <opcodes.h>
reg* (*FUNC_TABLE[FUNC_TABLE_SIZE])(int, int, int) = {add, mul, divide, sub, ld, sav, p, sto, set}; // Function table
int in_bounds(int i) {
  return i >= 0 && i < REGISTERTABLE_SIZE;
}
OPCODE_FUNC(set) {
  if (!in_bounds(car)) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    return err_reg;
  }
  reg* to = REGISTERTABLE[car];
  to->val = cadr + caddr;
  sprintf(to->debug, "-> %i", to->val);
  return to;
}
OPCODE_FUNC(add) {
  if(!in_bounds(car) || !in_bounds(cadr)) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    return err_reg;
  }
  reg* to = REGISTERTABLE[car];
  reg* from = REGISTERTABLE[cadr];
  to->val += from->val;
  sprintf(to->debug, "-> %i", to->val);
  return to;
}
OPCODE_FUNC(sub) {
  if(!in_bounds(car) || !in_bounds(cadr)) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    return err_reg;
  }
  reg* to = REGISTERTABLE[car];
  reg* from = REGISTERTABLE[cadr];
  to->val -= from->val;
  sprintf(to->debug, "-> %i", to->val);
  return to;
}
OPCODE_FUNC(mul) {

  if(!in_bounds(car) || !in_bounds(cadr)) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    return err_reg;
  }
  reg* to = REGISTERTABLE[car];
  reg* from = REGISTERTABLE[cadr];
  to->val *= from->val;
  sprintf(to->debug, "-> %i", to->val);
  return to;
}
OPCODE_FUNC(divide) {
  if(!in_bounds(car) || !in_bounds(cadr)) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    return err_reg;
  }
  reg* to = REGISTERTABLE[car];
  reg* from = REGISTERTABLE[cadr];
  if (from->val == 0) {
    err_reg->err = DIV_BY_ZERO_ERROR;
    return err_reg;
  }
  to->val /= from->val;
  sprintf(to->debug, "-> %i", to->val);
  return to;
}

OPCODE_FUNC(ld) {
  if(!in_bounds(car) || cadr > MEMTABLE_SIZE || cadr < 0) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    return err_reg;
  }
  reg* to = REGISTERTABLE[car];
  to = memmove(to, MEMTABLE[cadr], sizeof(reg));
  sprintf(to->debug, "-> SUCCESS");
  return to;
}

OPCODE_FUNC(sav) {
  if(!in_bounds(car) || cadr > MEMTABLE_SIZE || cadr < 0) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    return err_reg;
  }
  reg* from = REGISTERTABLE[car];
  reg* to = MEMTABLE[cadr];
  memcpy(to, from, sizeof(reg));
  sprintf(from->debug, "-> SUCCESS");
  return from;
}

OPCODE_FUNC(p) {
  if(!in_bounds(car)) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    return err_reg;
  }
  reg* regi = REGISTERTABLE[car];
  sprintf(regi->debug, "-> %i", regi->val);
  return regi;
}

OPCODE_FUNC(sto) {
  if(!in_bounds(car) || cadr > MEMTABLE_SIZE || cadr < 0) {
    err_reg->err = OUT_OF_BOUNDS_ERROR;
    return err_reg;
  }
  reg* from = REGISTERTABLE[car];
  reg* to = MEMTABLE[cadr];
  memcpy(to, from, sizeof(reg));
  memset(from, 0, sizeof(reg));
  sprintf(from->debug, "-> SUCCESS");
  from->val = 0;
  return from;
}
reg* new_register() {
  reg* ptr = malloc(sizeof(reg));
  ptr->val = 0;
  return ptr;
}
void init_asm_interpreter() {
  err_reg = malloc(sizeof(reg));
  for (int i = 0; i < REGISTERTABLE_SIZE; i++) {
    REGISTERTABLE[i] = new_register();
  }
  for (int i = 0; i < MEMTABLE_SIZE; i++) {
    MEMTABLE[i] = new_register();
  }
}
void free_asm_interpreter() {
  free(err_reg);
  for (int i = 0; i < REGISTERTABLE_SIZE; i++) {
    free(REGISTERTABLE[i]);
  }
  for (int i = 0; i < MEMTABLE_SIZE; i++) {
    free(MEMTABLE[i]);
  }
}
int ctoi(char c) {
  if ('0' <= c &&  c <= '9') {
    return c - '0';
  } else {
    return CTOI_ERR;
  }
}
void inplace_reverse(char * str)
{
  if (str)
    {
      char * end = str + strlen(str) - 1;
      // walk inwards from both ends of the string,
      // swapping until we get to the middle
      while (str < end)
        {
          char temp = *str;
          *str = *end;
          *end = temp;
          str++;
          end--;
        }
    }
}
int parse_binary(char* binary, int factor, int scale) {
  int power = strlen(binary) - 1;
  int ret = 0;
  if (scale)
    inplace_reverse(binary);
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
reg* parse_and_run(char* func, char* car, char* cadr, char* caddr) {
  int function, c, cd, cdd;
  function = parse_binary(func, 0, 0);
  if (function > FUNC_TABLE_SIZE) {
    err_reg->err = UNDEFINED_FUNCTION;
    return err_reg;
  }
  c = parse_binary(car, 0, 0);
  cd = parse_binary(cadr, function == 8 ? 4 : 0, 0);
  cdd = parse_binary(caddr, 0, 0);
  if (function == BINARY_ERR ||
      c == BINARY_ERR ||
      cd == BINARY_ERR ||
      cdd == BINARY_ERR) {
    err_reg->err = BINARY_ERR;
    return err_reg;
  }
  return (*FUNC_TABLE[function])(c, cd, cdd);
}
// TODO maybe function pointer table instead of these opcodes
