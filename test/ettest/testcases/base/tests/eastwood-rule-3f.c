// RUN: %%check_clang_tidy %%s eastwood-rule-3f %%t
/* --------------------- Test Code Style Section III.F 
 * Never place spaces between function names and the parenthesis preceding the a
 * rgument list.
 * ----------------------------------------------------------------- */
#include "eastwood-rule-3f.h"

/* 
 * A function
 */

int foo (void) {
} /* foo() */

/*
 * baz
 */

int baz (int i) {
} /* baz() */

/* 
 * A function
 */

int bar  (void) {
} /* bar() */

// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

/*
 * top header
 */

int xyzzy(int i) {
} /* xyzzy() */

// OK: Reason
// Some code that doesn't fail here
