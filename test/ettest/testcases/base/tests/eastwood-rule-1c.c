// RUN: %%check_clang_tidy %%s eastwood-rule-1c %%t
/* --------------------- Test Code Style Section I.C
 * All constants must be all uppercase, and contain at least two characters.  Co
 * nstants must be declared using #define. A constant numeric value assigned mus
 * t be enclosed in parenthesis. String constants need to be placed in quotes bu
 * t do not have surrounding parentheses.
 * ----------------------------------------------------------------- */
#include "eastwood-rule-1c.h"
#include <stdio.h>

#define ROOM_TEMPERATURE 10            // Bad: No parens
#define room_temperature (10)          // Bad: Not uppercase
#define ROOM_TEMPERATURE_AVG (10)      // Good: Uppercase and has parens
#define ROOM_TEMPERATURE_STRING ("10") // Bad: Do not use parens for strings
#define ROOM_TEMP "ten"                // Good: String constant in quotes

/*
 * main
 * @returns 0
 */

int main() {
  // Good: should print out when dumping embedded constants
  int some_thing = 10;
  // Good: should print out when dumping embedded constants
  printf("%d\n", some_thing + 5);
  // Good: should print out when dumping embedded constants
  return 1;
} /* main() */
