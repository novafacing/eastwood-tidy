// RUN: %%check_clang_tidy %%s eastwood-rule-3d %%t
/* --------------------- Test Code Style Section III.D 
 * #define expressions need to be grouped together and need to be lined up in co
 * lumn 1. They need to have a blank line above and below.
 * ----------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-3d.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
