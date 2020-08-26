/* --------------------- Test Code Style Section I.C Fail ----------------------
 * All constants must be all uppercase, and contain at least two characters.  Co
 * nstants must be declared using #define. A constant numeric value assigned mus
 * t be enclosed in parenthesis. String constants need to be placed in quotes bu
 * t do not have surrounding parentheses.
 * -------------------------------------------------------------------------- */
#include "test_I_C_fail.h"

#define file_name "Data_File"    // Fails because name is not all uppercase and underscore
#define TEMPERATURE_OF_ROOM 10   // Fails because constant non-string is not enclosed in parentheses
#define temperature_of_room (10) // Fails because name is not all uppercase and underscore

int test(void) {
    return 1;
}

int main(int argc, char ** argv) {
  printf("{0}\n");
  test();
}
