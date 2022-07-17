// RUN: %%check_clang_tidy %%s eastwood-rule-12c %%t
/* --------------------- Test Code Style Section XII.C
 * Variables should be placed in as local a scope as possible, as close to the f
 * irst use as possible.
 * ----------------------------------------------------------------- */
#include "eastwood-rule-12c.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
