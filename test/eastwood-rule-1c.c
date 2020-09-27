// RUN: %%check_clang_tidy %%s eastwood-rule-1c %%t
/* --------------------- Test Code Style Section I.C 
 * All constants must be all uppercase, and contain at least two characters.  Co
 * nstants must be declared using #define. A constant numeric value assigned mus
 * t be enclosed in parenthesis. String constants need to be placed in quotes bu
 * t do not have surrounding parentheses.
 * --------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-1c.h"

#define ROOM_TEMPERATURE 10
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 'ROOM_TEMPERATURE' initializer is non-string constant and not surrounded by parentheses. [eastwood-Rule1C]
//
#define room_temperature (10)
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 'room_temperature' is not all uppercase, separated by underscores, and >= 2 characters in length. [eastwood-Rule1C]

// OK: Reason
// Some code that doesn't fail here
#define ROOM_TEMPERATURE_AVG (10)
#define ROOM_TEMP "ten"
