// RUN: %%check_clang_tidy %%s eastwood-rule-12b %%t
/* --------------------- Test Code Style Section XII.B 
 * All variables must be initialized at the time they are  defined.
 * ------------------------------------------------------------------------------------------------------ */
#include "eastwood-rule-12b.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
