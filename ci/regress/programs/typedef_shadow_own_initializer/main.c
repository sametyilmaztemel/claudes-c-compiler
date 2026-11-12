/* The scope of a variable starts before its initializer, so sizeof(x) in the
 * initializer of x names the variable, not the typedef. */
#include <stdio.h>

typedef char x;
typedef short y;

static int f(void) {
    static int x = sizeof(x);
    return x;
}

int main(void) {
    int x = sizeof(x);
    long long y = sizeof(y) + ({ int t = sizeof(y); t; });
    printf("%d %lld %d\n", x, y, f());
    return x + (int)y + f();
}
