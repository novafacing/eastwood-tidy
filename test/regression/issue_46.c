#include <stdio.h>

int foo(char *);

int main() { foo("bar"); }

int foo(char *c) {
    int x = 0;
    printf("%s\n", c);
    return 0;
}
