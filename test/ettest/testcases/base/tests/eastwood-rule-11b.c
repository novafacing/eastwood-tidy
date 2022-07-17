// RUN: %%check_clang_tidy %%s eastwood-rule-11b %%t
/* --------------------- Test Code Style Section XI.B
 * Use only UNIX newline encoding (
). DOS newlines (
) are prohibited.
 * ----------------------------------------------------------------- */
#include "eastwood-rule-11b.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
