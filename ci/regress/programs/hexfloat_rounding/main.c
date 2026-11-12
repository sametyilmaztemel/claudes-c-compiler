/* A hexadecimal floating constant is rounded once, to nearest even, from its
 * exact value. Ties, values just above a tie, and the case where rounding in
 * several steps would give a different answer. */
#include <stdio.h>
#include <string.h>

static unsigned long long bits(double d) {
    unsigned long long u;
    memcpy(&u, &d, sizeof u);
    return u;
}

int main(void) {
    struct { const char *src; double value; unsigned long long expect; } t[] = {
        { "tie, even below",     0x1.00000000000008p0,           0x3ff0000000000000ULL },
        { "tie, even above",     0x1.00000000000018p0,           0x3ff0000000000002ULL },
        { "above tie",           0x1.00000000000008000000001p0,  0x3ff0000000000001ULL },
        { "2^53 + 1.5",          0x20000000000001.8p0,           0x4340000000000001ULL },
        { "2^63 + 1024 tie",     0x8000000000000400p0,           0x43e0000000000000ULL },
        { "2^63 + 3072 tie",     0x8000000000000c00p0,           0x43e0000000000002ULL },
        { "2^63 - 1",            0x7fffffffffffffffp0,           0x43e0000000000000ULL },
        { "54 bits, tie",        0x1.ffffffffffffe8p0,           0x3ffffffffffffffeULL },
        { "tie carries up",      0x1.fffffffffffff8p0,           0x4000000000000000ULL },
        { "tie broken by tail",  0x1.ffffffffffffe80000000000000001p0, 0x3fffffffffffffffULL },
    };
    int bad = 0;
    for (unsigned i = 0; i < sizeof t / sizeof t[0]; i++) {
        int ok = bits(t[i].value) == t[i].expect;
        printf("%-20s %016llx %s\n", t[i].src, bits(t[i].value), ok ? "ok" : "WRONG");
        bad += !ok;
    }
    return bad;
}
