// RUN: %%check_clang_tidy %%s eastwood-rule-9a %%t
/* --------------------- Test Code Style Section IX.A 
 * Return values of functions such as malloc, calloc, fopen, fread, fwrite, and 
 * system must be checked or returned whenever a possible error condition exists
 * .
 * ---------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-9a.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
