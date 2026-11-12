/* A block-scope declarator hides a typedef of the same name from the end of
 * its own declarator onwards (C11 6.2.1p7), including inside the initializers
 * of later declarators of the same declaration. */
#include <stdio.h>

typedef char x;

int main(void) {
    {
        int a = sizeof(x), x = 5, c = sizeof(x);
        printf("%d %d %d\n", a, x, c);
    }
    /* The block has ended: x is the typedef again. */
    printf("%d\n", (int)sizeof(x));
    return 0;
}
