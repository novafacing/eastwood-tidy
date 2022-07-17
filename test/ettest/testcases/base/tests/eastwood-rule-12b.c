// RUN: %%check_clang_tidy %%s eastwood-rule-12b %%t
/* --------------------- Test Code Style Section XII.B
 * All variables must be initialized at the time they are  defined.
 * ----------------------------------------------------------------- */
#include "eastwood-rule-12b.h"


int g_x; // Bad: RAII

int g_y = 0; // OK: RAII

/*
 * foo
 */

int foo(void) {
  int a; // Bad: RAII
} /* foo() */

// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here


/*
 * bar
 */

int bar(void) {
  int b = 5; // OK: RAII
} /* bar() */

// OK: Reason
// Some code that doesn't fail here
