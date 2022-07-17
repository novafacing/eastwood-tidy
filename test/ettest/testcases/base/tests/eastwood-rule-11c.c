// RUN: %%check_clang_tidy %%s eastwood-rule-11c %%t
/* --------------------- Test Code Style Section XI.C
 * Do not make more than one assignment per expression.
 * ----------------------------------------------------------------- */
#include "eastwood-rule-11c.h"

/*
 * foo
 */

int foo(void) {
  int a = 0;
  int b = 0;
  // Bad; two assignments in one expression
  if ((a = 10) > (b = 20)) {
    int c = 10;
  }
  // Bad: two assignments in one expression
  while (((a = a + 1) < 100) && ((b = b + 1) < 100)) {

  }
  // Bad: two assignments in one expression
  int x = a = b;
  // Bad: two assignments in one expression
  b = a = x;
} /* foo() */

// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

/*
 * bar
 */

int bar(void) {
  // OK: This is allowed in for loops (??????)
  for (int i = 0; i < 10; i++) {
  }
  int a = 5;
  // OK; only one assignment
  if ((a = 10) == 10) {
  }

  // OK: only one assignment
  while ((a = a + 1) < 100) {
  }
} /* bar() */

// OK: Reason
// Some code that doesn't fail here
