/* --------------------- Test Code Style Section I.B Fail ----------------------
 * Use descriptive and meaningful names.
 * -------------------------------------------------------------------------- */
#include "test_I_B_fail.h"

struct m { // Bad struct name
    int a;
    int b;
};

void b(int n) { // Bad function name
    return;
}

int test(void) {
    int x = 0; // Bad variable name
    int y = 100; // Bad variable name
    b(y);
    int thing = 10;
    char * dinger = "hello";
    return x;
}

int main(int argc, char ** argv) {
  printf("{0}\n");
  test();
}
