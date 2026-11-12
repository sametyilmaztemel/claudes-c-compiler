#include <stdio.h>
/* A tiny threaded interpreter: the entry points are static scalar label addresses. */
static int run(const unsigned char *code) {
    static void *start = &&op_next;
    static const void *halt = (const void *)&&op_halt;
    static void *tab[] = { &&op_inc, &&op_dbl, &&op_halt };
    int acc = 1;
    const unsigned char *pc = code;
    goto *start;
op_next:
    if (*pc == 9) goto *halt;
    goto *tab[*pc++];
op_inc: acc += 1; goto op_next;
op_dbl: acc *= 2; goto op_next;
op_halt: return acc;
}
int main(void) {
    unsigned char prog[] = { 0, 1, 1, 0, 9 };
    printf("%d\n", run(prog));
    return 0;
}
