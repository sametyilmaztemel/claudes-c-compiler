/* Subnormals, overflow and very large exponents of double hexadecimal
 * floating constants (C11 6.4.4.2p3: the result is correctly rounded, so too
 * large becomes infinity and too small becomes zero). */
#include <stdio.h>
#include <string.h>

static unsigned long long bits(double d) {
    unsigned long long u;
    memcpy(&u, &d, sizeof u);
    return u;
}

int main(void) {
    struct { const char *src; double value; unsigned long long expect; } t[] = {
        { "0x1p-1074",                 0x1p-1074,                 0x0000000000000001ULL },
        { "0x100p-1082",               0x100p-1082,               0x0000000000000001ULL },
        { "0x1.8p-1074",               0x1.8p-1074,               0x0000000000000002ULL },
        { "0x1p-1075",                 0x1p-1075,                 0x0000000000000000ULL },
        { "0x1.0000000000000001p-1075",0x1.0000000000000001p-1075,0x0000000000000001ULL },
        { "0x1.ffffffffffffep-1023",   0x1.ffffffffffffep-1023,   0x000fffffffffffffULL },
        { "0x1.fffffffffffff8p-1023",  0x1.fffffffffffff8p-1023,  0x0010000000000000ULL },
        { "0x0.0000000000001p1074",    0x0.0000000000001p1074,    0x7fd0000000000000ULL },
        { "0x0.8p1024",                0x0.8p1024,                0x7fe0000000000000ULL },
        { "0x1.fffffffffffff7ffp1023", 0x1.fffffffffffff7ffp1023, 0x7fefffffffffffffULL },
        { "0x1.fffffffffffff8p1023",   0x1.fffffffffffff8p1023,   0x7ff0000000000000ULL },
        { "0x1p99999999999999999999",  0x1p99999999999999999999,  0x7ff0000000000000ULL },
        { "0x1p-99999999999999999999", 0x1p-99999999999999999999, 0x0000000000000000ULL },
        { "0x1p4294967296",            0x1p4294967296,            0x7ff0000000000000ULL },
        { "0x1p-4294967296",           0x1p-4294967296,           0x0000000000000000ULL },
        { "-0x1p-1074",               -0x1p-1074,                 0x8000000000000001ULL },
    };
    int bad = 0;
    for (unsigned i = 0; i < sizeof t / sizeof t[0]; i++) {
        int ok = bits(t[i].value) == t[i].expect;
        printf("%-27s %016llx %s\n", t[i].src, bits(t[i].value), ok ? "ok" : "WRONG");
        bad += !ok;
    }
    return bad;
}
