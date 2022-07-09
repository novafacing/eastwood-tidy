// RUN: %%check_clang_tidy %%s eastwood-rule-4c %%t
/* --------------------- Test Code Style Section IV.C 
 * Do while loops need to be indented with the while on the same line as the clo
 * sing brace.
 * ---------------------------------------------------------------------------------- */
#include "eastwood-rule-4c.h"

void fun() {
    do {
        int a = 0;

    }
    while (0);
}

// OK: Reason
// Some code that doesn't fail here
void goodfun() {
    do {
        int a = 0;
    } while (0);
}

