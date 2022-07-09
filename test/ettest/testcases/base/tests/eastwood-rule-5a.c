// RUN: %%check_clang_tidy %%s eastwood-rule-5a %%t
/* --------------------- Test Code Style Section V.A 
 * Comment should be meaningful and not repeat the obvious. Comments are intende
 * d to alert people the intention of the code.
 * --------------------------------------------------------------------------------- */
#include "eastwood-rule-5a.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
