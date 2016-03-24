#if __gnu_linux__
#include <signal.h>
#endif
#include <opcodes.h>
#include <stdlib.h>
#include <stdio.h>
/* This is the cross platform cli client to the system */
#if __gnu_linux__
volatile sig_atomic_t done = 0;
void term(int signum)
{
  done = 1;
}

#endif
int main(int argc, char *argv[])
{
  init_asm_interpreter();
#if __gnu_linux__
  struct sigaction action;
  memset(&action, 0, sizeof(struct sigaction));
  action.sa_handler = term;
  sigaction(SIGINT, &action, NULL);
  while (!done) {
#else
   while (1) {
#endif
    char *func, *car, *cadr, *caddr;
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
