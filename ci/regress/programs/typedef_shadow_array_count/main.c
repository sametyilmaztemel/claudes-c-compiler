/* sizeof(x) / sizeof(*x) counts the elements of the local array x, which
 * hides a typedef of the same name that is a 7-byte struct. */
#include <stdio.h>

typedef struct { char c[7]; } x;

int main(void) {
    int x[5] = {10, 20, 30, 40, 50}, n = sizeof(x) / sizeof(*x), s = 0;
    for (int i = 0; i < n; i++)
        s += x[i];
    printf("%d %d\n", n, s);
    return n;
}
