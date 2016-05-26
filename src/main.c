/*
  main.c
  ---
  This file contains the main entrance to the program (see main()) and handles both Windows and Linux specific Command Line Interface code.

  This file makes heavy use of #if declarations, which are preprocessor (that is the compiler evaluates them) that are compiled out of the final program.

  Take for instance the following directive
  #if __gnu_linux__
      ...code...
  #endif
  The code inside that directive is only compiled in Linux, Windows doesn't have anything to do with it.

  This file makes use of the preprocessor to be able to provide cross platform support
  (by compiling only portions of code relevant to the specific operating system).
 */





#if __gnu_linux__
/*
  The "signal.h" header provides the ability to catch signals from the user (e.g if they attempt to kill the program through ^C).
  it provides the type sig_atomic_t, sigaction, and the macro SIGINT. It also provides sigaction() which sets up a handler for the events sent.
*/
#include <signal.h>
#elif __WIN32 || __WIN64
/*
  Windows.h provides similar functionality (and lots more besides we aren't needing) to singal.h in Linux.

  It provides the BOOL, WINAPI, TRUE and DWORD macros, as well as the function SetConsoleCtrlHandler.
 */
#include <windows.h>
#endif
#include <opcodes.h>
#include <stdlib.h>
#include <stdio.h>
/* This is the cross platform cli client to the system */
#if __gnu_linux__
/*
  done is a global volatile (meaning it can be edited by multiple threads) variable that tells the interpreter to quit the main loop and cleanup resources
 */
volatile sig_atomic_t done = 0;
/*  term is called when the user presses ^C. It updates the done variable to tell the interpreter to quit the main loop */
void term(int signum) {(void)signum; done = 1; }
#elif __WIN32 || __WIN64
/* done is a volatile int whose purpose is identical to the done variable in the linux code */
static volatile int done = 0;
/* this function is called when the user presses ^C in Windows, and tells the interpreter to quit the main loop */
static BOOL WINAPI console_ctrl_handler(DWORD dwCtrlType) {
  done = 1;
  /* Returning TRUE tells Windows we've handled the event, and it doesn't need to do anything itself */
  return TRUE;
}

#endif
/* maybe_print_error takes a register (returned from parse_and_run) and prints any errors the register has associated. */
void maybe_print_error(reg* r) {
  if (r->err) {
    /* Print to stderr (the standard error stream) rather than the regular one */
    fprintf(stderr, "%s\n", r->error);
  }
}
int main(int argc, char *argv[]) {
  (void)argc;(void)argv;
  /* See init_asm_interpreter in opcodes.c */
  init_asm_interpreter();
  /* In order:

     the following variables represent each section of inputted line passed to the interpreter

     (An example line)
     1000 0000 1111 1111
     maps each section (space separated blocks) to the following pattern
     func car  cadr caddr
     func represents the function to call
     car,cadr and caddr represent the arguments of that function, they are called such due to the naming scheme Common Lisp uses to denot the first, second, and third element of a list respectively. */
  char *func = NULL, *car = NULL, *cadr = NULL, *caddr = NULL;
#if __gnu_linux__
  /* This section of code (until the #elif ...) setups up the ^C handler on linux. */
  struct sigaction action;
  memset(&action, 0, sizeof(struct sigaction));
  action.sa_handler = term;
  sigaction(SIGINT, &action, NULL);
  while (!done) {
#elif __WIN32 || __WIN64
  /* This section of code sets up the ^C handler on Windows  */
  SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
  while (!done) {
#else
  while (1) {
#endif
    /* Allocate space for each of the variables.
       Calloc is used instead of malloc as calloc has the advantage of setting each portion of the allocated memory to 0, which is super important for strings who might have junk at the end if they aren't filled all the way.
     */
    func = calloc(4 + 1, sizeof(char));
    car = calloc(4 + 1, sizeof(char));
    cadr = calloc(4 + 1, sizeof(char));
    caddr = calloc(4 + 1, sizeof(char));
    /* Here is the actual prompt printed to the screen */
    printf("> ");
    /* each variable is parsed from scanf (see the scanf documentation for details on the formatting syntax) */
    int err = scanf("%5s %5s %5s %5s", func, car, cadr, caddr);
    /* if an error occurred while parsing OR the number of read sections was less than the required amount (4) */
    if (ferror(stdin) || err < 4) {
      fprintf(stderr, "\nASM: Error: Invalid input received\n");
    } else {
      /* see parse_and_run for more details */
      reg *res = parse_and_run(func, car, cadr, caddr);
#if __gnu_linux__ || __WIN32 || __WIN64
      /* For operating systems that support it, check user didn't press ^C and print the debug output (the feedback to the user) if they haven't */
      if (!done)
        puts(res->debug);
#else
      puts(res->debug);
#endif
      /* see maybe_print_error */
      maybe_print_error(res);
      res->err = 0; // reset the error code
      int c = 0;
      while ((c = fgetc(stdin)) != '\n' && c != EOF); /* Flush stdin */
    }
    /* Free all the various sections of the code (remembering we allocated at the start) */
    free(func); free(car); free(cadr); free(caddr);
    /* Set them to NULL so we can't accidentally attempt to free the same memory twice (this is called a double free error) */
    func = NULL; car = NULL; cadr = NULL; caddr = NULL;
  }
  /* Free each section */
  free(func); free(car); free(cadr); free(caddr);
  printf("\nExiting...\n");
  /* see free_asm_interpreter */
  free_asm_interpreter();
  return 0;
}
