#include "opcodes.h"
reg* (*FUNC_TABLE[8])(int, int, int) = {add, mul, divide, sub, ld, sav, p, sto}; // Function table
OPCODE_FUNC(add) {
  reg* to = REGISTERTABLE[car];
  reg* from = REGISTERTABLE[cadr];
  to->val += from->val;
  sprintf(to->debug, "-> divided two registers");
  return to;
}
OPCODE_FUNC(sub) {
  reg* to = REGISTERTABLE[car];
  reg* from = REGISTERTABLE[cadr];
  to->val -= from->val;
  sprintf(to->debug, "-> divided two registers");
  return to;
}
OPCODE_FUNC(mul) {
  reg* to = REGISTERTABLE[car];
  reg* from = REGISTERTABLE[cadr];
  to->val *= from->val;
  sprintf(to->debug, "-> divided two registers");
  return to;
}
OPCODE_FUNC(divide) {
  reg* to = REGISTERTABLE[car];
  reg* from = REGISTERTABLE[cadr];
  to->val /= from->val;
  sprintf(to->debug, "-> divided two registers");
  return to;
}

OPCODE_FUNC(ld) {
  reg* to = REGISTERTABLE[car];
  to = memmove(to, MEMTABLE[cadr], sizeof(reg));
  sprintf(to->debug, "-> divided two registers");
  return to;
}

OPCODE_FUNC(sav) {
  reg* from = REGISTERTABLE[car];
  reg* to = MEMTABLE[cadr];
  memcpy(to, from, sizeof(reg));
  sprintf(from->debug, "-> Saved register");
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
  sprintf(from->debug, "-> Stored register");
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
// TODO maybe function pointer table instead of these opcodes
