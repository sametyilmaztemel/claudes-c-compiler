/* Comparing _Alignof with a negative int converts the int to size_t
   (C11 6.3.1.8p1), so the int becomes huge and the comparison is unsigned. */
#include <stdio.h>

struct S { char c; double d; };

static int minus_one(void) { return -1; }
static int minus_five(void) { return -5; }

int main(void) {
    int m1 = minus_one();
    int m5 = minus_five();
    int hits = 0;
    int r;

    r = _Alignof(int) > m5;              printf("alignof > -5: %d\n", r);       hits += r;
    r = m5 < __alignof__(struct S);      printf("-5 < alignof: %d\n", r);       hits += r;
    r = m1 < _Alignof(int);              printf("-1 < alignof: %d\n", r);       hits += r;
    r = _Alignof(void *) > m1;           printf("alignof > -1: %d\n", r);       hits += r;
    r = -1 < _Alignof(int);              printf("literal -1: %d\n", r);         hits += r;
    r = _Alignof(int) < -1;              printf("alignof < -1: %d\n", r);       hits += r;
    printf("ternary: %d\n", _Alignof(int) > m1 ? 1 : 2);
    return hits;
}
