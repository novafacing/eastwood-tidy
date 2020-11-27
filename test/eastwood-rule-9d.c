// RUN: %%check_clang_tidy %%s eastwood-rule-9d %%t
/* --------------------- Test Code Style Section IX.D 
 * Appropriate range checking must be performed to make sure received parameters
 *  are within expected range.
 * ---------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-9d.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
