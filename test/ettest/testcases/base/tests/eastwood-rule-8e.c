// RUN: %%check_clang_tidy %%s eastwood-rule-8e %%t
/* --------------------- Test Code Style Section VIII.E 
 * Includes should be ordered per the following example. Suppose dir/foo.c imple
 * ments things in dir2/foo.h. Your includes should be ordered as follows:
 * ---------------------------------------------------------------------- */
#include "eastwood-rule-8e.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "c.h"
#include "b.h"
#include "a.h"

/*
 * a comment
 */

int foo(void) {
    int a = 10;
} /* foo() */

// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
