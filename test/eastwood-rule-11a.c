// RUN: %%check_clang_tidy %%s eastwood-rule-11a %%t
/* --------------------- Test Code Style Section XI.A 
 * Do not use tabs for indentation.
 * ----------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-11a.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
