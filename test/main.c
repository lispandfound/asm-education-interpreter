#include <opcodes.h>
#include <tap.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
  plan(45);
  /*
    Character to int tests
   */
  int c = ctoi('0');
  ok(c == 0, "Base test of character to integer conversion");
  c = ctoi('a');
  ok(c == CTOI_ERR, "Parsing error test");
  /*
    Parse Binary Test
  */
  int bin = parse_binary("1111", 0);
  ok(bin == 15, "Base test of binary numbers");
  bin = parse_binary("1111", 4);
  ok(bin == 240, "Test binary numbers where exponent is increased by 4");
  bin = parse_binary("asdc", 0);
  ok(bin == BINARY_ERR, "Test for binary parse error");
  /*
    Test initializing libasminterpreter
   */
  init_asm_interpreter();
  reg* r = NULL;
  /* Opcode Tests */
  /*
    In bounds test
   */
  ok(!in_bounds(-1), "In bounds check for below bounds");
  ok(!in_bounds(999), "In bounds check for above bounds");
  ok(in_bounds(0), "In bounds check for actually in bounds");
  /*
    Set
   */
  r = set(0, 12, 0);
  is(r->debug, "-> 12", "Test debug output for base case SET opcode");
  ok(!r->err, "Test no error for base case set");
  r = set(999, 0, 0);
  ok(r->err == OUT_OF_BOUNDS_ERROR, "Test error occurs for out of index access to register table");
  r = set(-1, 0, 0);
  ok(r->err == OUT_OF_BOUNDS_ERROR, "Test error occurs for (below bounds) out of index access to register table");
  /*
    Add
  */
  (void)set(0, 1, 0);
  (void)set(1, 1, 0);
  r = add(0, 1, 0);
  ok(r->val == 2, "Test addition is correct");
  ok(!r->err, "Test basic add case");
  r = add(-1, 999, 0);
  ok(r->err == OUT_OF_BOUNDS_ERROR, "Test out of bounds add case (above and below)");
  /*
    Sub
   */
  (void)set(0, 1, 0);
  (void)set(1, 1, 0);
  r = sub(0, 1, 0);
  ok(r->val == 0, "Test sub is correct");
  ok(!r->err, "Test basic sub case");
  r = sub(-1, 999, 0);
  ok(r->err == OUT_OF_BOUNDS_ERROR, "Test out of bounds sub case (above and below)");
  /*
    Multiply
  */
  (void)set(0, 1, 0);
  (void)set(1, 1, 0);
  r = mul(0, 1, 0);
  ok(r->val == 1, "Test mul is correct");
  ok(!r->err, "Test basic mul case");
  r = mul(-1, 999, 0);
  ok(r->err == OUT_OF_BOUNDS_ERROR, "Test out of bounds mul case (above and below)");
  /*
    Divide
  */
  (void)set(0, 1, 0);
  (void)set(1, 1, 0);
  r = divide(0, 1, 0);
  ok(r->val == 1, "Test divide is correct");
  ok(!r->err, "Test basic divide case");
  r = divide(-1, 999, 0);
  ok(r->err == OUT_OF_BOUNDS_ERROR, "Test out of bounds divide case (above and below)");
  (void)set(0, 0, 0);
  (void)set(1, 0, 0);
  r = divide(0, 1, 0);
  ok(r->err == DIV_BY_ZERO_ERROR, "Test divide by zero case");
  /*
    Save
  */
  r = sav(0, 0, 0);
  ok(!r->err, "Test basic save case");
  r = sav(0, 999, 0);
  ok(r->err == OUT_OF_BOUNDS_ERROR, "Test out of bounds save case (memtable)");
  r = sav(-1, 0, 0);
  ok(r->err == OUT_OF_BOUNDS_ERROR, "Test out of bounds save case (register table)");
  /*
    Load
   */
  (void)set(0, 123, 0);
  (void)sav(0, 0, 0);
  r = ld(0, 0, 0);
  ok(!r->err);
  ok(r->val == 123, "Test basic ld case");
  r = ld(0, 999, 0);
  ok(r->err == OUT_OF_BOUNDS_ERROR, "Test out of bounds ld case (memtable)");
  r = ld(-1, 0, 0);
  ok(r->err == OUT_OF_BOUNDS_ERROR, "Test out of bounds ld case (register table)");
  /*
    Store
   */
  r = sto(0, 0, 0);
  ok(!r->err, "Test basic sto case");
  ok(!r->val, "Test destructive store properties");
  r = sto(0, 999, 0);
  ok(r->err == OUT_OF_BOUNDS_ERROR, "Test out of bounds sto case (memtable)");
  r = sto(-1, 0, 0);
  ok(r->err == OUT_OF_BOUNDS_ERROR, "Test out of bounds sto case (register table)");
  /*
    Print String
   */
  char* str_to_save = "Hello World";
  int len = strlen(str_to_save);
  for (int i = 0; i < len; i++) {
    (void)set(0, *(str_to_save + i), 0);
    (void)sav(0, i, 0);
  }
  (void)set(0, 0, 0);
  (void)set(1, len, 0);
  r = p_str(0, 1, 0);
  is(r->debug, "-> Hello World", "Test Base Case Print String");
  r = p_str(1, 0, 0);
  ok(r->err == INVALID_STR_LENGTH_ERROR, "Test negative length string print string case");
  r = p_str(0, 0, 0);
  ok(r->err == INVALID_STR_LENGTH_ERROR, "Test empty string case");
  r = p_str(3, 1, 0);
  ok(r->err == OUT_OF_BOUNDS_ERROR, "Test Out of Bounds Error");
  (void)set(0, 0, 0);
  (void)set(1, 21, 0);
  r = p_str(0, 1, 0);
  ok(r->err == INVALID_STR_LENGTH_ERROR, "Test Out of Bounds Error (range too large)");
  /*
    Parsing
  */
  r = parse_and_run("1000", "0000", "0000", "0001");
  ok(r->val == 1, "Test basic parse case (set register 1 to value 1)");
  r = parse_and_run("1000", "0000", "1000", "0001");
  ok(r->val == 129, "Test advanced parsing test (set register 1 to value 129, using all of the available bytes for set)");
  r = parse_and_run("ErrorPlease", "0000", "0000", "0000");
  ok(r->err, "Test garbage value in parsing");
  r = parse_and_run("1111", "0000", "0000", "0000");
  ok(r->err, "Test out of bounds error for function");
  free_asm_interpreter();
  done_testing();
}
