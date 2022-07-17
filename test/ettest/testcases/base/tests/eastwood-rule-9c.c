// RUN: %%check_clang_tidy %%s eastwood-rule-9c %%t
/* --------------------- Test Code Style Section IX.C 
 * After dynamically deallocating a pointer using free, the pointer must be set 
 * back to NULL.
 * ----------------------------------------------------------------- */
#include "eastwood-rule-9c.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
