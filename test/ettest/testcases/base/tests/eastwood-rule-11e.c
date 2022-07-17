// RUN: %%check_clang_tidy %%s eastwood-rule-11e %%t
/* --------------------- Test Code Style Section XI.E
 * The use of goto is forbidden in this course.
 * ----------------------------------------------------------------- */
#include "eastwood-rule-11e.h"

void badgoto() {
    goto end;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: Use of goto is prohibited.

end:
    return;
}


// OK: anything without GOTO is gonna work so I'm leaving this blank
// Some code that doesn't fail here
