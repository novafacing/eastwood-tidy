// RUN: %%check_clang_tidy %%s eastwood-rule-12b %%t
/* --------------------- Test Code Style Section XII.B 
 * All variables must be initialized at the time they are  defined.
 * ------------------------------------------------------------------------------------------------------ */
#include "eastwood-rule-12b.h"


int g_x; // Bad: RAII

int foo(void) {
    int a; // Bad: RAII
}
// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

int g_y = 0;

int bar(void) {
    int b = 5;
}
// OK: Reason
// Some code that doesn't fail here
