/* A float hexadecimal constant is rounded directly to binary32, not first to
 * double. A double constant narrowed to float is rounded twice, as C requires. */
#include <stdio.h>
#include <string.h>

static unsigned bits(float f) {
    unsigned u;
    memcpy(&u, &f, sizeof u);
    return u;
}

int main(void) {
    struct { const char *src; float value; unsigned expect; } t[] = {
        { "0x1.0000010000000001p0f",         0x1.0000010000000001p0f,         0x3f800001u },
        { "0x1.00000100000000000000001p0F",  0x1.00000100000000000000001p0F,  0x3f800001u },
        { "0x10000000000000000p-64f",        0x10000000000000000p-64f,        0x3f800000u },
        { "0x1.000001p0f (tie)",             0x1.000001p0f,                   0x3f800000u },
        { "0x1.000003p0f (tie)",             0x1.000003p0f,                   0x3f800002u },
        { "0x1.ffffffp127f (overflow)",      0x1.ffffffp127f,                 0x7f800000u },
        { "0x1.fffffep127f (FLT_MAX)",       0x1.fffffep127f,                 0x7f7fffffu },
        { "0x1p-149f",                       0x1p-149f,                       0x00000001u },
        { "0x1.8p-149f",                     0x1.8p-149f,                     0x00000002u },
        { "0x1p-150f",                       0x1p-150f,                       0x00000000u },
        { "(float)0x1.0000010000000001p0",   (float)0x1.0000010000000001p0,   0x3f800000u },
    };
    int bad = 0;
    for (unsigned i = 0; i < sizeof t / sizeof t[0]; i++) {
        int ok = bits(t[i].value) == t[i].expect;
        printf("%-32s %08x %s\n", t[i].src, bits(t[i].value), ok ? "ok" : "WRONG");
        bad += !ok;
    }
    return bad;
}
