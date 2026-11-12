#include <stdio.h>
static int run(int k) {
    static void *one = &&A;
    static void *const two = (void *)&&B;
    static void *tab[] = { &&A, &&B };
    static struct { void *l; int v; } rec = { &&B, 40 };
    if (k == 0) goto *one;
    if (k == 1) goto *two;
    if (k == 2) goto *tab[1];
    goto *rec.l;
A:  return 1;
B:  return 2 + (k == 3 ? rec.v : 0);
}
int main(void){ printf("%d %d %d %d\n", run(0), run(1), run(2), run(3)); return 0; }
