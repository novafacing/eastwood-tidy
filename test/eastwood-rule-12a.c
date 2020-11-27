// RUN: %%check_clang_tidy %%s eastwood-rule-12a %%t
/* --------------------- Test Code Style Section XII.A 
 * No more than one variable may be defined on a single line.
 * ------------------------------------------------------------------------------------------------------ */
#include "eastwood-rule-12a.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
