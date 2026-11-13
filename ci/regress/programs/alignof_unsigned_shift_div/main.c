/* Shift, division and remainder of a wrapped _Alignof difference must use
   unsigned semantics: a logical shift, an unsigned divide and an unsigned
   remainder. Results are reduced to booleans so they do not depend on the
   width of size_t. */
#include <stdio.h>

static int five(void) { return 5; }
static int eight(void) { return 8; }

int main(void) {
    int n5 = five();
    int n8 = eight();
    int hits = 0;
    int r;

    r = ((_Alignof(int) - 5) >> 1) > 1000;      printf("const shift: %d\n", r);      hits += r;
    r = ((_Alignof(int) - 8) / 2) > 1000;       printf("const div: %d\n", r);        hits += r;
    r = ((_Alignof(int) - 5) % 2) == 1;         printf("const rem: %d\n", r);        hits += r;
    r = ((_Alignof(int) - n5) >> 1) > 1000;     printf("runtime shift: %d\n", r);    hits += r;
    r = ((_Alignof(int) - n8) / 2) > 1000;      printf("runtime div: %d\n", r);      hits += r;
    r = ((_Alignof(int) - n5) % 2) == 1;        printf("runtime rem: %d\n", r);      hits += r;
    r = (2 * _Alignof(int) - 9) > 100;          printf("scaled: %d\n", r);           hits += r;
    return hits;
}
