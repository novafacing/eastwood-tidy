// RUN: %%check_clang_tidy %%s eastwood-rule-5e %%t
/* --------------------- Test Code Style Section V.E
 * Function header comments should have a blank line above and below the comment
 * .
 * ----------------------------------------------------------------- */
#include "eastwood-rule-5e.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
