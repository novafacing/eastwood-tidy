// RUN: %%check_clang_tidy %%s eastwood-rule-3e %%t
/* --------------------- Test Code Style Section III.E 
 * Never put trailing whitespace at the end of a line.
 * ----------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-3e.h"

int main() {
  // Bad, the next line has a trailing whitespace
  int b = 10; 
  // Bad, the next line has a trailing whitespace
  if (1) { 
    // Bad: the next line has a trailing whitespace
    int c = 10; 
    // Bad: the next line has a trailing whitespace
    while (1) { 
      // Bad: the next line has a trailing whitespace
      int d = 10; 
      // Bad: the next line has a trailing whitespace
      switch (d) { 
        // Bad: the next line has a trailing whitespace
        case 1: 
          // Bad: the next line has a trailing whitespace
          d = 10; 
          // Bad: The next line C-style comment has a trailing whitespace
          // This comment has a trailing whitespace 
          // and this continues it.
          // Bad: The next line C++-style comment has a trailing whitespace
          /* this line is a c++-style comment and it has a trailing whitespace 
           * and this continues it. */
          // Bad: The next line has a trailing whitespace
          break; 
        // Bad: the next line has a trailing whitespace
        default: { 
          // Bad: the next line has a trailing whitespace
          d = 11; 
          // Bad: the next line has a trailing whitespace
          break; 
        // Bad: the next line has a trailing whitespace
        } 
      // Bad: the next line has a trailing whitespace
      } 
    // Bad: the next line has a trailing whitespace
    } 
  // Bad: the next line has a trailing whitespace
  } 
// Bad: the next line has a trailing whitespace
} 

// Good: No trailing whitespaces here
int bar(void) {
  int a = 10;
}