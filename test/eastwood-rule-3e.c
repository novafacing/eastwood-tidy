// RUN: %%check_clang_tidy %%s eastwood-rule-3e %%t
/* --------------------- Test Code Style Section III.E 
 * Never put trailing whitespace at the end of a line.
 * ----------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-3e.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
