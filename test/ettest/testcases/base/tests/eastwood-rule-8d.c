// RUN: %%check_clang_tidy %%s eastwood-rule-8d %%t
/* --------------------- Test Code Style Section VIII.D 
 * All project header files should be listed as descendants of the project's sou
 * rce directory. UNIX directory shortcuts  (e.g., . and ..) are forbidden.
 * ------------------------------------------------------------------------------------------------------ */
#include "./eastwood-rule-8d.h"
#include "../test/eastwood-rule-8x.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

#include "eastwood-rule-8d.h"
// OK: Reason
// Some code that doesn't fail here
