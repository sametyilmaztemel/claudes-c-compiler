/* Every use of plain char follows one signedness, the target's: loads, character
   constants, string literals, static initializers, enum constants, switch,
   bit-fields, arguments and return values, arithmetic and comparisons. */
#include <stdio.h>
#if defined(__aarch64__) || defined(__riscv)
#define WANT_UNSIGNED 1
#else
#define WANT_UNSIGNED 0
#endif
/* Evaluated while parsing, before the code generator is involved. */
#if WANT_UNSIGNED
_Static_assert((char)200 == 200 && (char)-1 > 0, "plain char is unsigned");
#else
_Static_assert((char)200 == -56 && (char)-1 < 0, "plain char is signed");
#endif
static const int k = (char)0x80;
static char g = (char)0xE9;
static char s[] = "\xf0";
enum { E1 = '\xfe' };
struct bf { char a : 3; };
__attribute__((noinline)) static char ret_c(int x) { return (char)x; }
__attribute__((noinline)) static int take_c(char c) { return c; }
static int sw(char c) {
    switch (c) {
    case '\xff': return 1;
    default: return 0;
    }
}
int main(void) {
    int u = WANT_UNSIGNED;
    volatile int v = 0xF0;
    char c = (char)v;
    struct bf b;
    b.a = 7;
    printf("%d\n", u == ((char)-1 > 0));
    printf("%d\n", '\xff' == (u ? 255 : -1));
    printf("%d\n", "\x80"[0] == (u ? 128 : -128));
    printf("%d\n", k == (u ? 128 : -128));
    printf("%d\n", g == (u ? 233 : -23));
    printf("%d\n", s[0] == (u ? 240 : -16));
    printf("%d\n", E1 == (u ? 254 : -2));
    printf("%d\n", b.a == (u ? 7 : -1));
    printf("%d\n", ret_c(200) == (u ? 200 : -56));
    printf("%d\n", take_c(c) == (u ? 240 : -16));
    printf("%d\n", sw((char)v) == 0 && sw((char)0xff) == 1);
    printf("%d\n", c / 16 == (u ? 15 : -1));
    printf("%d\n", (c >> 4) == (u ? 15 : -1));
    printf("%d\n", (long long)c == (u ? 240LL : -16LL));
    printf("%d\n", (c < 0) == !u);
    printf("%d\n", (signed char)0xF0 == -16 && (unsigned char)0xF0 == 240);
    return 0;
}
