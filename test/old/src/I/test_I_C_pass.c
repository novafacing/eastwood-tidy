/* --------------------- Test Code Style Section I.C Pass ----------------------
 * All constants must be all uppercase, and contain at least two characters.  Co
 * nstants must be declared using #define. A constant numeric value assigned mus
 * t be enclosed in parenthesis. String constants need to be placed in quotes bu
 * t do not have surrounding parentheses.
 * -------------------------------------------------------------------------- */
#include "test_I_C_pass.h"

#define TEMPERATURE (10)         // OK because all uppercase, constant non-string enclosed in parens
#define TEMPERATURE_OF_ROOM (10) // OK because all upper and underscore, enclosed in parens
#define STATIC_STR "Hello"       // OK because all upper and underscore, string const enclosed in quotes

int test(void) {

}

int main(int argc, char ** argv) {
  printf("{0}\n");
  test();
}
