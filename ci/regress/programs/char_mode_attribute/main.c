/* __attribute__((mode(..))) on plain char takes its signedness from plain
 * char, as GCC does. The output does not depend on the target. */
#include <stdio.h>

typedef char cq __attribute__((mode(QI)));
typedef char ch __attribute__((mode(HI)));
typedef signed char sq __attribute__((mode(QI)));
typedef unsigned char uh __attribute__((mode(HI)));

#ifdef __CHAR_UNSIGNED__
#define PLAIN_IS_NEGATIVE 0
#define QI_TAG 2
#define HI_TAG 2
#else
#define PLAIN_IS_NEGATIVE 1
#define QI_TAG 1
#define HI_TAG 1
#endif

int main(void)
{
    cq a = (cq)200;
    ch h = (ch)-1;
    sq s = (sq)200;
    uh u = (uh)-1;
    int ok = 1;
    ok = ok && ((a < 0) == PLAIN_IS_NEGATIVE);
    ok = ok && ((h < 0) == PLAIN_IS_NEGATIVE);
    ok = ok && (s < 0);
    ok = ok && (u > 0);
    ok = ok && (_Generic(a, signed char: 1, unsigned char: 2, default: 0) == QI_TAG);
    ok = ok && (_Generic(h, short: 1, unsigned short: 2, default: 0) == HI_TAG);
    ok = ok && (_Generic(s, signed char: 1, default: 0) == 1);
    ok = ok && (_Generic(u, unsigned short: 1, default: 0) == 1);
    printf("mode attribute follows plain char: %d\n", ok);
    return !ok;
}
