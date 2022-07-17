// RUN: %%check_clang_tidy %%s eastwood-rule-9b %%t
/* --------------------- Test Code Style Section IX.B 
 * When the function fopen is invoked to open a file, fclose must  be used to cl
 * ose the file.  It is important to remember that fclose does not explicitly se
 * t the file pointer back to NULL. Therefore, it is necessary to set the file p
 * ointer to NULL.
 * ----------------------------------------------------------------- */
#include "eastwood-rule-9b.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
