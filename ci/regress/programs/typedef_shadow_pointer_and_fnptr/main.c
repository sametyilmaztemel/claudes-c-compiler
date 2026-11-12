/* Pointer and function-pointer declarators that reuse a typedef name. */
#include <stdio.h>

typedef char x;
typedef short T;

static int two(int v) { return v * 2; }

int main(void) {
    int *x = 0, n = sizeof(x) == sizeof(void *), m = sizeof(*x);
    int (*T)(int) = two, r = T(4), q = (T)(5);
    printf("%d %d %d %d\n", n, m, r, q);
    return r + q;
}
