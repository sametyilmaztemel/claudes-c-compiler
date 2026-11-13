/* Byte-oriented code that mixes plain char with explicit masks and casts: the
   values that depend on the signedness of plain char must agree with
   (char)-1 < 0, and the values that do not depend on it must be the same on
   every target. Every printed line is 1 when the compiler is consistent. */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

static unsigned checksum_masked(const char *p, int n) {
    unsigned s = 0;
    while (n--) s = s * 31u + (unsigned)(*p++ & 0xff);
    return s;
}
static unsigned checksum_cast(const char *p, int n) {
    unsigned s = 0;
    while (n--) s = s * 31u + (unsigned char)*p++;
    return s;
}
static unsigned checksum_plain(const char *p, int n) {
    unsigned s = 0;
    while (n--) s = s * 31u + (unsigned)*p++;
    return s;
}
static int count_high(const char *p, int n) {
    int k = 0;
    while (n--) if (*p++ < 0) k++;
    return k;
}
static int cmp_bytes(const char *a, const char *b) {
    while (*a && *a == *b) { a++; b++; }
    return *a - *b;
}

int main(void) {
    static const char data[] = "ab\x80\xc3\xa9\xff z\x7f";
    int n = (int)sizeof data - 1;
    int uns = (char)-1 > 0;
    int n_high = 0;
    for (int i = 0; i < n; i++) n_high += ((unsigned char)data[i] >= 0x80);

    printf("%d\n", checksum_masked(data, n) == checksum_cast(data, n));
    printf("%d\n", (checksum_plain(data, n) == checksum_cast(data, n)) == (uns || n_high == 0));
    printf("%d\n", count_high(data, n) == (uns ? 0 : n_high));
    printf("%d\n", (cmp_bytes("a\xe9", "a") > 0) == 1 || !uns);
    printf("%d\n", (cmp_bytes("a\xe9", "a") > 0) == uns);
    printf("%d\n", (strcmp("a\xe9", "a") > 0) == 1);
    printf("%d\n", uns == (CHAR_MIN == 0));
    printf("%d\n", uns == (CHAR_MAX == UCHAR_MAX));
#ifdef __CHAR_UNSIGNED__
    printf("%d\n", uns == 1);
#else
    printf("%d\n", uns == 0);
#endif
    char buf[8];
    memcpy(buf, "\xfe\x01", 2);
    printf("%d\n", (buf[0] + buf[1]) == (uns ? 255 : -1));
    printf("%d\n", isalpha((unsigned char)data[0]) && !isalpha((unsigned char)data[2]));
    return 0;
}
