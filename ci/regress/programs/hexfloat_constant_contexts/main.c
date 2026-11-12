/* Wide hexadecimal floating constants in the places a compiler folds them:
 * static initializers, integer constant expressions, and constant arithmetic
 * and comparisons. */
#include <stdio.h>

enum { A = (int)0x10000000000000000p-60, B = (int)0x1.8p1 };
static char buf[(int)0x20000000000000000p-61];
static double dtab[] = { 0x10000000000000000p-64, 0x1p-1074 * 2, 0x123456789abcdef0123p-72 };
static float ftab[] = { 0x1.0000010000000001p0f, 0x10000000000000000p-64f };
static const double kOne = 0x10000000000000000p-64;

int main(void) {
    volatile double one = 1.0;
    printf("enum %d %d\n", A, B);
    printf("sizeof %d\n", (int)sizeof buf);
    printf("dtab %d %d %d\n", dtab[0] == 1.0, dtab[1] == 0x1p-1073, dtab[2] == 0x1.23456789abcdfp-0);
    printf("ftab %d %d\n", ftab[0] == 0x1.000002p0f, ftab[1] == 1.0f);
    printf("fold %d %d\n", 0x10000000000000000p-64 + 0x10000000000000000p-64 == 2.0,
           0x10000000000000000p-64 == 1.0);
    printf("run %d %d\n", kOne * one == 1.0, (int)(0x1.fffffffffffffffffffp4) == 32);
    return !(A == 16 && B == 3 && sizeof buf == 16 && dtab[0] == 1.0 && ftab[1] == 1.0f && kOne == 1.0);
}
