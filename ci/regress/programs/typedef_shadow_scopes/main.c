/* Shadowing follows block structure: inner blocks, a re-typedef and a
 * function-local variable all resolve to the right entity. */
#include <stdio.h>

typedef char x;

static int f(void) {
    int x = 7;
    return sizeof(x);
}

int main(void) {
    int total = 0;
    printf("%d\n", (int)sizeof(x));
    {
        int x = 1, a = sizeof(x);
        {
            typedef double x;
            printf("%d\n", (int)sizeof(x));
        }
        printf("%d %d\n", x, a);
        total += a;
    }
    printf("%d %d\n", f(), (int)sizeof(x));
    return total + f();
}
