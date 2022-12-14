// RUN: %%check_clang_tidy %%s eastwood-rule-12a %%t
/* --------------------- Test Code Style Section XII.A
 * No more than one variable may be defined on a single line.
 * ----------------------------------------------------------------- */
#include "eastwood-rule-12a.h"


// This is OK, there will be no initializer for extern vars
extern int g_x;

// Bad, multiple declarations on one line
int g_y = 100, g_z = 10;

/*
 * foo
 */

int foo(void) {
  int a = 10, b = 20; // Bad, multiple declarations on one line
  int c, d; // Bad, multiple declarations on one line.
} /* foo() */


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

/*
 * bar
 */

int bar(void) {
  // These are OK; there is only one declaration per line
  int a = 10;
  int b = 20;
  // These should fail for other tests but should not fail 12a
  int c = 1;
  int d = 1;
} /* bar() */

// OK: Reason
// Some code that doesn't fail here
