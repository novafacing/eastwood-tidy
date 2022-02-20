// RUN: %%check_clang_tidy %%s eastwood-rule-10a %%t
/* --------------------- Test Code Style Section X.A 
 * All error messages must be directed to standard error.
 * ---------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-10a.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
