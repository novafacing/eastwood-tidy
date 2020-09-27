// RUN: %%check_clang_tidy %%s eastwood-rule-1a %%t

/* --------------------- Test Code Style Section I.A  --------------------------
 * Variable names should be in all lowercase. If the name is composed of more th
 * an one word, then underscores must be used to separate them.
 * -------------------------------------------------------------------------- */

int roomTemperature = 0;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: int roomTemperature is not all lowercase and separated by underscores [eastwood-Rule1A]

int roomtemperature = 0;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: int roomtemperature is not all lowercase and separated by underscores [eastwood-Rule1A]

int Room_Temperature = 0;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: int Room_Temperature is not all lowercase and separated by underscores [eastwood-Rule1A]

// OK: Matches the format
int room_temperature = 0;

// OK: Matches the fomrat
int temperature = 0;
