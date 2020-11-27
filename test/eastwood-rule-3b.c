// RUN: %%check_clang_tidy %%s eastwood-rule-3b %%t
/* --------------------- Test Code Style Section III.B 
 * One space must be placed before and after all logical, and  arithmetic operat
 * ors; except for unary and data reference  operators (i.e. [], ., &, *, ->).
 * ----------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-3b.h"


// Put Something That Fails Here
int bad(){
    // Bad: no spacing
    int a = 10+4;
    // Bad: no spacing
    if (a==14){
        // Bad: no spacing
        if (a==14&&10==10){
            // Bad: no spacing
            int i = (5,10);
        }
    }
    return a;
}
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
int good() {
    // Bad: no spacing
    int a = 10 + 4;
    // Bad: no spacing
    if (a == 14) {
        // Bad: no spacing
        if (a == 14 && 10 == 10) {
            // Bad: no spacing
            int i = (5, 10);
        }
    }
    return a;
}
// Some code that doesn't fail here
