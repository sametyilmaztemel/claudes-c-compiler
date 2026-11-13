#include <stdio.h>

/* The shift count has its own type: an unsigned __int128 count makes the
   shift 128 bits wide, but the signed left operand is still promoted by its
   own type, so a negative value stays negative. */
#ifdef __SIZEOF_INT128__
typedef unsigned __int128 count_t;
#else
typedef unsigned long long count_t;
#endif

int main(void) {
    volatile long a = -8;
    volatile long long b = -1000000000000LL;
    volatile int c = -4096;
    volatile short d = -300;
    count_t one = 1, five = 5, three = 3;
    long x = a;
    long long y = b;
    int z = c;
    short w = d;

    x >>= one;
    y >>= five;
    z >>= three;
    w >>= one;
    printf("x = %ld\n", x);
    printf("y = %lld\n", y);
    printf("z = %d\n", z);
    printf("w = %d\n", w);
    return (int)((-x + (-z) / 64 + (-w) / 10) & 0x7f);
}
