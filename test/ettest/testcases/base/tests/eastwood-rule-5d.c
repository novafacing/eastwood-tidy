// RUN: %%check_clang_tidy %%s eastwood-rule-5d %%t
/* --------------------- Test Code Style Section V.D
 * Function's name must be commented at the end of each function. The comment sh
 * ould be the name of the function indented one space after the closing brace a
 * nd include left and right parentheses.
 * --------------------------------------------------------------------------- */
#include "eastwood-rule-5d.h"


int foo(void) {
}

int baz(void) {
    // Bad: wrong name
} /* foo() */

int boo(void) {
    // Bad: too much spacing
}  /* boo() */

// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

int bar(void) {
} /* bar() */

// OK: Reason
// Some code that doesn't fail here
