// RUN: %%check_clang_tidy %%s eastwood-rule-4c %%t
/* --------------------- Test Code Style Section IV.C 
 * Do while loops need to be indented with the while on the same line as the clo
 * sing brace.
 * ---------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-4c.h"

void fun() {
    do {
        int a = 0;

    }
    while (0);
// CHECK-MESSAGES: :[[@LINE-2]]:6: warning: While statement in a do-while must be on same line as the closing brace. [eastwood-Rule4C]
}

// OK: Reason
// Some code that doesn't fail here
void goodfun() {
    do {
        int a = 0;
    } while (0);
}

