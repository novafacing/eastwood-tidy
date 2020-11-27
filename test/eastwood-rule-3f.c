// RUN: %%check_clang_tidy %%s eastwood-rule-3f %%t
/* --------------------- Test Code Style Section III.F 
 * Never place spaces between function names and the parenthesis preceding the a
 * rgument list.
 * ----------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-3f.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
