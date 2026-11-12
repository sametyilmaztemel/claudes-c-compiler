/* With the typedef hidden, "(x) - 1" is a subtraction and "(x) * 3" a
 * multiplication, not casts of -1 and a dereference of 3. */
#include <stdio.h>

typedef long long x;

int main(void) {
    int x = 3, y = (x) - 1, z = (x) * 3, w = (x) + 5;
    printf("%d %d %d %d\n", x, y, z, w);
    if (1) {
        int x = 2, v = (x) * 7;
        printf("%d\n", v);
    }
    return y + z + w;
}
