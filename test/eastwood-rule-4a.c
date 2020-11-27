// RUN: %%check_clang_tidy %%s eastwood-rule-4a %%t
/* --------------------- Test Code Style Section IV.A 
 * Two space indentation must be applied to all structure,  control, and flow co
 * mmands.  This two space indentation rule must be applied to the entire progra
 * m Note that the opening brace must be placed on the same line as the structur
 * e, control, or flow command.  The closing brace must be placed on the line af
 * ter the structure, control, or flow commands. The closing brace must also be 
 * alone on the line. Even if only one statement is to be executed it is necessa
 * ry to use braces.
 * ---------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-4a.h"


// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

// OK: Reason
// Some code that doesn't fail here
