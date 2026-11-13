/* Plain char is signed on x86-64 and i686 and unsigned on AArch64 and
   RISC-V; __CHAR_UNSIGNED__, CHAR_MIN and CHAR_MAX agree with the code. */
#include <stdio.h>
#include <limits.h>
#if defined(__aarch64__) || defined(__riscv)
#define WANT_UNSIGNED 1
#else
#define WANT_UNSIGNED 0
#endif
int main(void) {
    volatile int v = 200;
    char c = (char)v;
    int uns = (char)-1 > 0;
#ifdef __CHAR_UNSIGNED__
    int mac = 1;
#else
    int mac = 0;
#endif
    printf("%d\n", uns == WANT_UNSIGNED);
    printf("%d\n", mac == WANT_UNSIGNED);
    printf("%d\n", CHAR_MIN == (WANT_UNSIGNED ? 0 : -128));
    printf("%d\n", CHAR_MAX == (WANT_UNSIGNED ? 255 : 127));
    printf("%d\n", c == (WANT_UNSIGNED ? 200 : -56));
#if CHAR_MIN == 0
    printf("%d\n", WANT_UNSIGNED == 1);
#else
    printf("%d\n", WANT_UNSIGNED == 0);
#endif
    return 0;
}
