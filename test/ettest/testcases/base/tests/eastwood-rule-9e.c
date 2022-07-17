// RUN: %%check_clang_tidy %%s eastwood-rule-9e %%t
/* --------------------- Test Code Style Section IX.E
 * The appropriately typed symbol should be used to represent 0 and NULL.
 * ----------------------------------------------------------------- */
#include "eastwood-rule-9e.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
