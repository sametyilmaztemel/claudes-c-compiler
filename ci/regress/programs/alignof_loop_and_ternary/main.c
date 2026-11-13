/* _Alignof in a loop bound, a conditional expression and mixed arithmetic
   with int operands, where the common type must be size_t. */
#include <stdio.h>

static int three(void) { return 3; }

int main(void) {
    int t3 = three();
    int count = 0;
    int i;

    /* The bound wraps to a huge size_t, so only `i < 3` stops the loop. */
    for (i = 0; i < _Alignof(char) - 2 && i < 3; i++)
        count++;
    printf("loop iterations: %d\n", count);

    /* Common type of size_t and int is size_t, so -1 becomes huge. */
    printf("conditional: %d\n", (0 ? _Alignof(int) : -1) > 0);
    printf("comma: %d\n", (0, _Alignof(char)) - 2 > 0);
    printf("int minus alignof: %d\n", t3 - _Alignof(int) > 0);
    printf("uchar minus alignof: %d\n", (unsigned char)3 - _Alignof(int) > 0);
    return count + ((t3 - _Alignof(int)) > 0);
}
