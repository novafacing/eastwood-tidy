// RUN: %%check_clang_tidy %%s eastwood-rule-6a %%t
/* --------------------- Test Code Style Section VI.A 
 * If multiple logical expressions are used, sub-expressions must be parenthesiz
 * ed. Note the spacing and format below.
 * ---------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-6a.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
