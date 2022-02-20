// RUN: %check_clang_tidy %s eastwood-rule-1a %t
/* --------------------- Test Code Style Section I.A 
 * Variable names should be in all lowercase. If the name is composed of more th
 * an one word, then underscores must be used to separate them.
 * --------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-1a.h"


int g_roomTemperature = 0;
// CHECK-MESSAGES: [[@LINE-1]]:5: warning: Variable g_roomTemperature is not all lowercase and separated by underscores [eastwood-Rule1A]

int g_Room_Temperature = 0;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: Variable g_Room_Temperature is not all lowercase and separated by underscores [eastwood-Rule1A]

// OK: Reason
// Some code that doesn't fail here
int g_room_temperature = 0;
