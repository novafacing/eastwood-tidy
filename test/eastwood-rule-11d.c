// RUN: %%check_clang_tidy %%s eastwood-rule-11d %%t
/* --------------------- Test Code Style Section XI.D 
 * Do not use embedded constants; except for general initialization purposes and
 *  values that lack intrinsic meaning. Common sense need to apply.
 * ----------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-11d.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
