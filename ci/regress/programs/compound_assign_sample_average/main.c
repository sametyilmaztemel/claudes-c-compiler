#include <stdio.h>

/* Averaging signed 16-bit and 8-bit samples by an unsigned count. */
static short average(const short *v, unsigned n) {
    short total = 0;
    unsigned i;
    for (i = 0; i < n; i++) total += v[i];
    total /= n;
    return total;
}

static signed char average8(const signed char *v, unsigned n) {
    signed char total = 0;
    unsigned i;
    for (i = 0; i < n; i++) total += v[i];
    total /= n;
    return total;
}

static short remainder(const short *v, unsigned n, unsigned long long m) {
    short total = 0;
    unsigned i;
    for (i = 0; i < n; i++) total += v[i];
    total %= m;
    return total;
}

int main(void) {
    static const short samples[] = { -1200, -800, -3000, 400, -2500, -100 };
    static const signed char small[] = { -10, -20, -30, -7 };
    printf("avg16 = %d\n", average(samples, 6));
    printf("avg16 head = %d\n", average(samples, 2));
    printf("avg8 = %d\n", average8(small, 4));
    printf("avg8 pair = %d\n", average8(small, 2));
    printf("rem16 = %d\n", remainder(samples, 6, 7));
    return average(samples, 6) < 0 ? 3 : 0;
}
