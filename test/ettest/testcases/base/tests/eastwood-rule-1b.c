// RUN: %%check_clang_tidy %%s eastwood-rule-1b %%t
/* --------------------- Test Code Style Section I.B 
 * Use descriptive and meaningful names.
 * --------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-1b.h"

// No check messages in this file because this is subjective.

// Put Something That Fails Here
// This is a bad name
int a = 1;

// This is also a bad name
int bnfunc() {
    return 1;
}

// OK: This is a good name
int room_temp = 0;
// Some code that doesn't fail here
