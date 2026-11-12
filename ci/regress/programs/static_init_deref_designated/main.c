#include <stdio.h>
static int a(void){ return 1; } static int b(void){ return 2; }
struct ops { int x; int (*f)(void); int (*g)(void); };
struct ops o = { .g = *b, .f = **a, .x = 9 };
struct ops o2 = { .f = &*b, .g = *&a };
int main(void){ printf("%d %d %d %d %d\n", o.x, o.f(), o.g(), o2.f(), o2.g()); return 0; }
