// RUN: %%check_clang_tidy %%s eastwood-rule-8b %%t
/* --------------------- Test Code Style Section VIII.B
 * Header files should be self-contained (compile on their own) and end in .h.
 * ----------------------------------------------------------------- */
#include "eastwood-rule-8b.hh"
#include "eastwood-rule-8b.hpp"
#include "eastwood-rule-8b.h"




// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
