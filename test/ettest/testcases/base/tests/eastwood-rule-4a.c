// RUN: %%check_clang_tidy %%s eastwood-rule-4a %%t
/* --------------------- Test Code Style Section IV.A 
 * Two space indentation must be applied to all structure,  control, and flow co
 * mmands.  This two space indentation rule must be applied to the entire progra
 * m Note that the opening brace must be placed on the same line as the structur
 * e, control, or flow command.  The closing brace must be placed on the line af
 * ter the structure, control, or flow commands. The closing brace must also be 
 * alone on the line. Even if only one statement is to be executed it is necessa
 * ry to use braces.
 * ----------------------------------------------------------------- */
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
    int a = 5;
      int b = 10;
    switch (a) {
      case 1:
        case 2:
          break;
      case 3:
      case 4:
      a = 10;
      b = 12;
      break;
      case 5:
      break;
      case 6:
      case 7: {
        break;
      }
    }
} /* foo() */

// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

/*
 * good 2
 */

int foo2(void) {
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
  }
  else {
    int a = 10;
  }
  int a = 5;
  int b = 10;
  switch (a) {
    case 1:
    case 2:
      break;
    case 3:
    case 4:
      a = 10;
      b = 12;
      break;
    case 5:
      break;
    case 6:
    case 7: {
      break;
    }
  }
} /* foo2() */

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
      case 9: break;
      case 8:
        break;
      case 7:
      case 6:
      case 5:
        break;
      case 4:
      case 3:
      case 2: break;
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
