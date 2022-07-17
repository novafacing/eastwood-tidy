// RUN: %%check_clang_tidy %%s eastwood-rule-8f %%t
/* --------------------- Test Code Style Section VIII.F
 * All relevant include files should be explicitly included. Do not assume that 
 * just because foo.h currently includes bar.h you do not need to explicitly inc
 * lude bar.h.
 * ----------------------------------------------------------------- */
#include "eastwood-rule-8f.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
