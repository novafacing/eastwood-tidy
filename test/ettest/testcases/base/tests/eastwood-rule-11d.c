// RUN: %%check_clang_tidy %%s eastwood-rule-11d %%t
/* --------------------- Test Code Style Section XI.D
 * Do not use embedded constants; except for general initialization purposes and
 *  values that lack intrinsic meaning. Common sense need to apply.
 * ------------------------------------------------------------------- */
#include "eastwood-rule-11d.h"

/*
 * a comment
 */

int foo(void) {
    int a = 10;
    int b = a + 100;
    if (a + b < 135) {
        return 1;
    }
    return 0;
} /* foo() */

// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
