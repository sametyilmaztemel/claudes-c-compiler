/* Hexadecimal floating constants whose digit strings need more than 64 bits
 * (C11 6.4.4.2): the value is the exact digit string rounded once. */
#include <stdio.h>
#include <string.h>

static unsigned long long bits(double d) {
    unsigned long long u;
    memcpy(&u, &d, sizeof u);
    return u;
}

int main(void) {
    struct { const char *src; double value; unsigned long long expect; } t[] = {
        { "0x10000000000000000p-64",       0x10000000000000000p-64,       0x3ff0000000000000ULL },
        { "0x1.80000000000000000p0",       0x1.80000000000000000p0,       0x3ff8000000000000ULL },
        { "0x123456789abcdef0123p0",       0x123456789abcdef0123p0,       0x44723456789abcdfULL },
        { "0xffffffffffffffffffffp-80",    0xffffffffffffffffffffp-80,    0x3ff0000000000000ULL },
        { "0x0.0000000000000000000000000000000000000000001p+172",
          0x0.0000000000000000000000000000000000000000001p+172,          0x3ff0000000000000ULL },
        { "0x1.000000000000000000000000000000000000000000000000001p0",
          0x1.000000000000000000000000000000000000000000000000001p0,     0x3ff0000000000000ULL },
    };
    int bad = 0;
    for (unsigned i = 0; i < sizeof t / sizeof t[0]; i++) {
        int ok = bits(t[i].value) == t[i].expect;
        printf("%s -> %016llx %s\n", t[i].src, bits(t[i].value), ok ? "ok" : "WRONG");
        bad += !ok;
    }
    return bad;
}
