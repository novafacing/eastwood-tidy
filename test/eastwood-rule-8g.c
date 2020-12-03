// RUN: %%check_clang_tidy %%s eastwood-rule-8g %%t
/* --------------------- Test Code Style Section VIII.G 
 * Non-local includes (standard libraries, etc) must be surrounded by < and >. D
 * ouble quotes should only be used for include files that exist in the local di
 * rectory structure.
 * ------------------------------------------------------------------------------------------------------ */
#include <eastwood-rule-8g.h>
#include "stdio.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
