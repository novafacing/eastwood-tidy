// RUN: %%check_clang_tidy %%s eastwood-rule-5b %%t
/* --------------------- Test Code Style Section V.B 
 * Place comments above the code rather than along side the code. Exceptions can
 *  be made for short comments placed beside declarations, else, and switch comm
 * ands.
 * --------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-5b.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
