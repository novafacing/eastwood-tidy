// RUN: %%check_clang_tidy %%s eastwood-rule-5c %%t
/* --------------------- Test Code Style Section V.C 
 * Code and comments must be separated by blank lines, and lined up. A blank lin
 * e is not required above the comment if it is the 1st. line following an openi
 * ng brace.
 * ----------------------------------------------------------------- */
#include "eastwood-rule-5c.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
