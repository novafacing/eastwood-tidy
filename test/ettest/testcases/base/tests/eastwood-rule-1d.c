// RUN: %%check_clang_tidy %%s eastwood-rule-1d %%t
/* --------------------- Test Code Style Section I.D 
 * All global variables must be started with prefix 'g_'.Declarations/definition
 * s should be at the top of the file.
 * --------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-1d.h"


int bad_global = 0;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: Global variable 'bad_global' doesn't conform to global naming scheme. [eastwood-Rule1D]

// OK: Reason
// Some code that doesn't fail here
int g_good_global = 0;

