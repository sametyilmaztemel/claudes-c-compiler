/* A typedef name is usable by the later declarators of its own declaration,
 * at file scope and at block scope. */
#include <stdio.h>

typedef double A, (*B)(A);
typedef int N, M[(N)2.5 + 1];
typedef short S, SA[sizeof(S) * 8];

static double half(double v) { return v / 2; }

int main(void) {
    B b = half;
    M m;
    SA sa;
    typedef char C, CA[sizeof(C) * 6];
    typedef int L, (*LF)(L, L);
    CA ca;
    printf("%g %d %d %d\n", b(3), (int)sizeof(m), (int)sizeof(sa), (int)sizeof(ca));
    return (int)sizeof(m);
}
