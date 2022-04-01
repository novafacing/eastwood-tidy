// RUN: %%check_clang_tidy %%s eastwood-rule-1d %%t
/* --------------------- Test Code Style Section I.D 
 * All global variables must be started with prefix 'g_'.Declarations/definition
 * s should be at the top of the file.
 * --------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-1d.h"


// Bad: not started with g_
int bad_global = 0;

// Good: started with g_
int g_good_global = 0;


int main() {
    return 0;
} /* main() */

// Bad: not at top of file
int g_another_bad_global = 0;
