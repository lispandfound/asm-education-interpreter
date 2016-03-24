#include "opcodes.h"
reg* (*FUNC_TABLE[9])(int, int, int) = {add, mul, divide, sub, ld, sav, p, sto, set}; // Function table
OPCODE_FUNC(set) {
  reg* to = REGISTERTABLE[car];
  to->val = cadr;
  sprintf(to->debug, "-> %i", to->val);
  return to;
}
OPCODE_FUNC(add) {
  reg* to = REGISTERTABLE[car];
  reg* from = REGISTERTABLE[cadr];
  to->val += from->val;
  sprintf(to->debug, "-> %i", to->val);
  return to;
}
OPCODE_FUNC(sub) {
  reg* to = REGISTERTABLE[car];
  reg* from = REGISTERTABLE[cadr];
  to->val -= from->val;
  sprintf(to->debug, "-> %i", to->val);
  return to;
}
OPCODE_FUNC(mul) {
  reg* to = REGISTERTABLE[car];
  reg* from = REGISTERTABLE[cadr];
  to->val *= from->val;
  sprintf(to->debug, "-> %i", to->val);
  return to;
}
OPCODE_FUNC(divide) {
  reg* to = REGISTERTABLE[car];
  reg* from = REGISTERTABLE[cadr];
  to->val /= from->val;
  sprintf(to->debug, "-> %i", to->val);
  return to;
}

OPCODE_FUNC(ld) {
  reg* to = REGISTERTABLE[car];
  to = memmove(to, MEMTABLE[cadr], sizeof(reg));
  sprintf(to->debug, "-> SUCCESS");
  return to;
}

OPCODE_FUNC(sav) {
  reg* from = REGISTERTABLE[car];
  reg* to = MEMTABLE[cadr];
  memcpy(to, from, sizeof(reg));
  sprintf(from->debug, "-> SUCCESS");
  return from;
}

OPCODE_FUNC(p) {
  reg* regi = REGISTERTABLE[car];
  sprintf(regi->debug, "-> %i", regi->val);
  return regi;
}

OPCODE_FUNC(sto) {
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
  for (int i = 0; i < REGISTERTABLE_SIZE; i++) {
    REGISTERTABLE[i] = new_register();
  }
  for (int i = 0; i < MEMTABLE_SIZE; i++) {
    MEMTABLE[i] = new_register();
  }
}
void free_asm_interpreter() {
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
    exit(EXIT_FAILURE);
  }
}
int parse_binary(char* binary) {
  int power = strlen(binary) - 1;
  int ret = 0;
  for (char *p = binary; *p != '\0'; p++) {
    if (ctoi(*p)) {
      ret += pow(2, power);
    }
    power--;
  }
  return ret;
}
reg* parse_and_run(char* func, char* car, char* cadr, char* caddr) {
  int function, c, cd, cdd;
  function = parse_binary(func);
  c = parse_binary(car);
  cd = parse_binary(cadr);
  cdd = parse_binary(caddr);
  return (*FUNC_TABLE[function])(c, cd, cdd);
}
// TODO maybe function pointer table instead of these opcodes
