// RUN: %%check_clang_tidy %%s eastwood-rule-5b %%t

/* --------------------- Test Code Style Section V.B
 * Place comments above the code rather than along side the code. Exceptions can
 *  be made for short comments placed beside declarations, else, and switch comm
 * ands.
 * ---------------------------------------------------------------------- */

#include "eastwood-rule-5b.h"

/*
 * This comment has a good tcomment....i promise
 */

int foo(void) {

  /* This comment doesn't have tail whitespace */
  int a = 5;
  /* This comment doesn't have preceeding whitespace */

  while (0) { /* You can't put a comment next to a while */
  }

  for (int i = 0; i < 1; i++) { /* Or a for */
  }

  switch (a) { /* or next to a switch */
    default: {
    }
  }
} /* foo() */

// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

/*
 * Proper function comment
 */

int bar(void) {
  /* This comment doesn't have preceeding whitespace.
   * It is ok because it follows a "{" */

  /* A note about this function. OK: has spacing */

  int a = 0;

  if (a == 0) { /* You can annotate here */

  }
  else { /* Here is ok too */
  }
} /* bar() */

// OK: Reason

// Some code that doesn't fail here
