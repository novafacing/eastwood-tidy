// RUN: %%check_clang_tidy %%s eastwood-rule-11c %%t
/* --------------------- Test Code Style Section XI.C
 * Do not make more than one assignment per expression. 
 * ----------------------------------------------------------------- */
#include "eastwood-rule-11c.h"


int foo(void) {
    int a = 0;
    int b = 0;
    if ((a = 10) > (b = 20)) { // Bad; two assignments in one expression
        int c = 10;
    }
    while ((a = a + 1) < 100 && (b = b + 1) < 100) { // Bad: two assignments in one expression

    }
    int x = a = b; // Bad: two assignments in one expression
    b = a = x; // Bad: two assignments in one expression
}
// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

int bar(void) {
    // OK: This is allowed in for loops (??????)
    for (int i = 0, j = 5; i < 10; j++, i++) {
    }
    int a = 5;
    // OK; only one assignment
    if ((a = 10) == 10) {
    }

    // OK: only one assignment
    while ((a = a + 1) < 100) {
    }
}
// OK: Reason
// Some code that doesn't fail here
