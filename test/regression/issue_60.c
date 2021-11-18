#include <stdio.h>

#define PREFIX (1)
#define POSTFIX (2)
#define FORWARD (3)
#define REVERSE (4)

#define VALID (1)
#define INVALID (0)

int traverse_order_check_good(int traverse_order) {
    if ((traverse_order == PREFIX) || (traverse_order == POSTFIX) ||
        (traverse_order == FORWARD) || (traverse_order == REVERSE)) {
        return VALID;
    }
    return INVALID;
}

int traverse_order_check_bad(int traverse_order) {
    if (traverse_order == PREFIX || traverse_order == POSTFIX ||
        traverse_order == FORWARD || traverse_order == REVERSE) {
        return VALID;
    }
    return INVALID;
}

int main() {
    int traverse_order = PREFIX;
    traverse_order_check(traverse_order);
}
