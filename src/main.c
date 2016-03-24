#include <opcodes.h>
#include <stdio.h>
/* This is the cross platform cli client to the system */
int main(int argc, char *argv[])
{
  init_asm_interpreter();
  while (1) {
    char *func, *car, *cadr, *caddr;
    func = malloc(4);
    car = malloc(4);
    cadr = malloc(4);
    caddr = malloc(4);
    func = calloc(4 + 1, sizeof(char));
    car = calloc(4 + 1, sizeof(char));
    cadr = calloc(4 + 1, sizeof(char));
    caddr = calloc(4 + 1, sizeof(char));
    printf("> ");
    scanf("%s %s %s %s", func, car, cadr, caddr);
    reg* res = parse_and_run(func, car, cadr, caddr);
    puts(res->debug);
    free(func);
    free(car);
    free(cadr);
    free(caddr);
  }
  free_asm_interpreter();
  return 0;
}
