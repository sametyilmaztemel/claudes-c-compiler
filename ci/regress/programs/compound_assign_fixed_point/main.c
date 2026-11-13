#include <stdio.h>

/* 16.16 style fixed point and scaling where the scale factor is unsigned. */
typedef int fix;

static fix from_int(int v) { return v * 256; }

static fix scale_down(fix v, unsigned long long divisor) {
    v /= divisor;
    return v;
}

static fix frac_part(fix v, unsigned long long unit) {
    v %= unit;
    return v;
}

static short gain(short s, unsigned num, unsigned den) {
    s *= num;
    s /= den;
    return s;
}

int main(void) {
    fix a = from_int(-5);
    fix b = from_int(3);
    printf("a = %d, b = %d\n", a, b);
    printf("a / 3 = %d\n", scale_down(a, 3));
    printf("b / 3 = %d\n", scale_down(b, 3));
    printf("a %% 256 = %d\n", frac_part(a - 17, 256));
    printf("b %% 256 = %d\n", frac_part(b + 17, 256));
    printf("gain(-300, 3, 7) = %d\n", gain(-300, 3, 7));
    printf("gain(300, 3, 7) = %d\n", gain(300, 3, 7));
    return scale_down(a, 3) < 0 ? 5 : 0;
}
