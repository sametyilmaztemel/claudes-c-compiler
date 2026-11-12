/* Long double hexadecimal constants with more digits than the significand
 * holds: the exponent must still count every digit, and the surplus digits
 * only take part in rounding. Results are compared, not printed, so that the
 * output does not depend on the long double format. */
#include <stdio.h>

int main(void) {
    int r[5];
    r[0] = 0x10000000000000000p-64L == 1.0L;
    r[1] = 0x10000000000000000000000000000000000p0L == 0x1p136L;
    r[2] = 0xffffffffffffffffffffffffffffffffffp-136L == 1.0L;
    r[3] = 0x1.000000000000000000000000000000000001p0L == 1.0L;
    r[4] = 0x1.0000000000000000000000000000000000000000001p0L == 1.0L;
    for (int i = 0; i < 5; i++)
        printf("r[%d] = %d\n", i, r[i]);
    int bad = 0;
    for (int i = 0; i < 5; i++)
        bad += !r[i];
    return bad;
}
