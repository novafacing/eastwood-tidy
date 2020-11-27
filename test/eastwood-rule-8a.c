// RUN: %%check_clang_tidy %%s eastwood-rule-8a %%t
/* --------------------- Test Code Style Section VIII.A 
 * In general, every .c file should have an associated .h file.
 * ------------------------------------------------------------------------------------------------------ */
#include "eastwood-rule-8a.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
