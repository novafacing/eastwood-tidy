// RUN: %%check_clang_tidy %%s eastwood-rule-8c %%t
/* --------------------- Test Code Style Section VIII.C
 * All header files should have #define guards to prevent multiple inclusions. T
 * he format of the symbol name should be _H.
 * ----------------------------------------------------------------- */
#include "eastwood-rule-8x.h"

#include <stdio.h>
#include <time.h>

#include "eastwood-rule-8c.h"

// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
