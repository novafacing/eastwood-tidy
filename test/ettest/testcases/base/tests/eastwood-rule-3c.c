// RUN: %%check_clang_tidy %%s eastwood-rule-3c %%t
/* --------------------- Test Code Style Section III.C 
 * One space must be placed after internal semi-colons and commas. 
 * ----------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-3c.h"
#include <stdio.h>

int foo(int a,int b) {

    /* Bad: No space after commas */

    printf("%d %d\n",a,b);

    for (int i = 0;i < 10;i++) {
        /* Bad: No space after semicolon */

    }

}

// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
