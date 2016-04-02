#if __gnu_linux__
#include <signal.h>
#elif __WIN32 || __WIN64
#include <windows.h>
#endif
#include <opcodes.h>
#include <stdlib.h>
#include <stdio.h>
/* This is the cross platform cli client to the system */
#if __gnu_linux__
volatile sig_atomic_t done = 0;
void term(int signum) {(void)signum; done = 1; }
#elif __WIN32 || __WIN64
static volatile int done = 0;
static BOOL WINAPI console_ctrl_handler(DWORD dwCtrlType) {
  done = 1;
  return TRUE;
}

#endif
void maybe_print_error(reg* r) {
  if (r->err) {
    fprintf(stderr, "%s\n", r->error);
  }
}
int main(int argc, char *argv[]) {
  (void)argc;(void)argv;
  init_asm_interpreter();
  char *func = NULL, *car = NULL, *cadr = NULL, *caddr = NULL;
#if __gnu_linux__
  struct sigaction action;
  memset(&action, 0, sizeof(struct sigaction));
  action.sa_handler = term;
  sigaction(SIGINT, &action, NULL);
  while (!done) {
#elif __WIN32 || __WIN64
  SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
  while (!done) {
#else
  while (1) {
#endif
    func = calloc(4 + 1, sizeof(char));
    car = calloc(4 + 1, sizeof(char));
    cadr = calloc(4 + 1, sizeof(char));
    caddr = calloc(4 + 1, sizeof(char));
    printf("> ");
    int err = scanf("%5s %5s %5s %5s", func, car, cadr, caddr);
    if (ferror(stdin) || err < 4) {
      // Error occurred during scanning
      fprintf(stderr, "\nASM: Error: Invalid input received\n");
    } else {
      reg *res = parse_and_run(func, car, cadr, caddr);
#if __gnu_linux__ || __WIN32 || __WIN64
      if (!done)
        puts(res->debug);
#else
      puts(res->debug);
#endif
      maybe_print_error(res);
      res->err = 0; // reset the error code
      int c = 0;
      while ((c = fgetc(stdin)) != '\n' && c != EOF); /* Flush stdin */
    }
    free(func); free(car); free(cadr); free(caddr);
    func = NULL; car = NULL; cadr = NULL; caddr = NULL;
  }
  free(func); free(car); free(cadr); free(caddr);
  printf("\nExiting...\n");
  free_asm_interpreter();
  return 0;
}
