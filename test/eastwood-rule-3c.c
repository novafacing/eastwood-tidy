// RUN: %%check_clang_tidy %%s eastwood-rule-3c %%t
/* --------------------- Test Code Style Section III.C 
 * One space must be placed after internal semi-colons and commas. 
 * ----------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-3c.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
