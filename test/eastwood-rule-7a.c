// RUN: %%check_clang_tidy %%s eastwood-rule-7a %%t
/* --------------------- Test Code Style Section VII.A 
 * A header must be placed at the beginning of each function (including the main
 *  program).  A header must contain detailed information, whic describes the pu
 * rpose of the function.  The format is defined below The header comment block 
 * must be at the left edge.
 * ----------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-7a.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
