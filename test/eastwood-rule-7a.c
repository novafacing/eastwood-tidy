// RUN: %%check_clang_tidy %%s eastwood-rule-7a %%t
/* --------------------- Test Code Style Section VII.A 
 * A header must be placed at the beginning of each function (including the main
 *  program).  A header must contain detailed information, whic describes the pu
 * rpose of the function.  The format is defined below The header comment block 
 * must be at the left edge.
 * ----------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-7a.h"


int foo(void) {
    // Bad: no function header comment
} /* foo() */

/*
 * The function does a thing. It is very good
 * Yes this function is great. This is a dummy comment for bar
 */
int bar(void) {
    // Bad: function header comment does not have a post-space.
} /* bar() */
/*
 * The function does a thing. It is very good
 * Yes this function is great. This is a dummy comment for baz
 */

int baz(void) {
    // Bad: function header comment does not have a pre-space
} /* baz() */

/* This is a badly formatted header regex
 */

int xyzzy(void) {
} /* xyzzy() */

// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

/*
 * The function does a thing. It is very good
 * Yes this function is great. This is a dummy comment for baz
 */

int zzyzzy(void) {
    // OK: Space above and below, properly formatted.
} /* zzyzzy() */

// OK: Reason
// Some code that doesn't fail here
