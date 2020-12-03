// RUN: %%check_clang_tidy %%s eastwood-rule-4a %%t
/* --------------------- Test Code Style Section IV.A 
 * Two space indentation must be applied to all structure,  control, and flow co
 * mmands.  This two space indentation rule must be applied to the entire progra
 * m Note that the opening brace must be placed on the same line as the structur
 * e, control, or flow command.  The closing brace must be placed on the line af
 * ter the structure, control, or flow commands. The closing brace must also be 
 * alone on the line. Even if only one statement is to be executed it is necessa
 * ry to use braces.
 * ---------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-4a.h"

typedef struct a
{
  int a;
  int b;
  int c;
};

/* 
 * Header comment
 */

int foo(void)
{
    if (1)
    {
        while (1)
        {
            int a = 10;
        }
        do
        {
            int a = 10;
        } while (1);
    } else if (0)
    {
        int a = 10;
    } else
    {
        int a = 10;
    }
} /* foo() */

// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

/*
 * good header comment
 */

int bar(void) {
  if (1) {
    while (1) {
      int a = 10;
    }
    do {
      int a = 10;
    } while (1);
  }
  else if (0) {
    int a = 10;
    switch (a) {
      case 10: {
        a = 5;
        break;
      }
      default: {
        a = 2;
        break;
      }
    }
  }
  else {
    int a = 10;
  }
} /* bar() */

// OK: Reason
// Some code that doesn't fail here
