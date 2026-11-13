/* char, signed char and unsigned char are three distinct types on every
   target, whatever the signedness of plain char. */
#include <stdio.h>
#include <stdint.h>
#define K(x) _Generic((x), char: 1, signed char: 2, unsigned char: 3, default: 0)
#define P(x) _Generic((x), char *: 1, signed char *: 2, unsigned char *: 3, default: 0)
typedef int qi_t __attribute__((mode(QI)));
struct sca { signed char m[3]; };
static signed char gs[] = "\x80\x7f";
int main(void) {
    char c = 0; signed char sc = 0; unsigned char uc = 0;
    int8_t i8 = 0; qi_t q = 0;
    __typeof__(sc) tsc = 0;
    printf("%d %d %d\n", K(c), K(sc), K(uc));
    printf("%d %d %d\n", K(i8), K(q), K(tsc));
    printf("%d %d %d %d\n", K("x"[0]), P("x"), P(&c), P(&sc));
    printf("%d %d %d\n", __builtin_types_compatible_p(char, signed char),
           __builtin_types_compatible_p(char, unsigned char),
           __builtin_types_compatible_p(signed char, unsigned char));
    sc = (signed char)200; i8 = (int8_t)200; q = (qi_t)200;
    printf("%d %d %d\n", sc, i8, q);
    signed char ls[] = "\xff";
    struct sca st = { "\x90" };
    signed char *cl = (signed char[]){ "\xfe" };
    printf("%d %d %d %d %d %d\n", gs[0], gs[1], (int)sizeof gs, ls[0], st.m[0], cl[0]);
    return 0;
}
