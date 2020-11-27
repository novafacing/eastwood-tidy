// RUN: %%check_clang_tidy %%s eastwood-rule-11c %%t
/* --------------------- Test Code Style Section XI.C 
 * Do not make more than one assignment per expression. 
 * ----------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-11c.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
