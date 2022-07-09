// RUN: %%check_clang_tidy %%s eastwood-rule-4b %%t
/* --------------------- Test Code Style Section IV.B 
 * Parameters for functions with more than one parameter should be on the same l
 * ine, unless the line length is exceeded. In that case, parameters on the next
 *  line should begin at the same column position as the parameters on the first
 *  line. The example below uses fewer than 80 characters just for demonstration
 *  purposes.
 * -------------------------------------------------------------------------------- */
#include "eastwood-rule-4b.h"

/*
 * good comment
 */

int foo(int aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,
         char * bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb,
          int xxxxxxxxxxxxxxxxxxxxxxxxxxx) {

    /* BAD: Does not have lined-up indentation */

} /* foo() */

// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

/*
 * yet another good comment
 */

int bar(int aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,
        char * bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb,
        int xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx) {


} /* bar() */

// OK: Reason
// Some code that doesn't fail here
