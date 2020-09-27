/* --------------------- Test Code Style Section I.B Pass ----------------------
 * Use descriptive and meaningful names.
 * -------------------------------------------------------------------------- */
#include "test_I_B_pass.h"

struct llist_node { // good struct name
    struct llist_node * n;
    int a;
};

int send_message() { // good function name
    printf("Hello!\n");
}

int test(void) {
    int refrigerator_uses = 0; // Good variable name
    int people_seen = 100; // Good nonzero variable name

}

int main(int argc, char ** argv) {
  printf("{0}\n");
  test();
}
