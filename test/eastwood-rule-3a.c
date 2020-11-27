// RUN: %%check_clang_tidy %%s eastwood-rule-3a %%t
/* --------------------- Test Code Style Section III.A 
 * One space must be placed after all structure control, and flow commands. One 
 * space must also be present between the closing parenthesis and opening brace.
 * ----------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-3a.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
