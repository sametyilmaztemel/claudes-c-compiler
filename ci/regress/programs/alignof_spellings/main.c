/* _Alignof and its spellings have type size_t (C11 6.5.3.4p5), so
   subtracting past zero wraps to a large unsigned value instead of -1. */
#include <stdio.h>
#include <stdalign.h>

int main(void) {
    char c = 0;
    int hits = 0;
    int r;

    r = _Alignof(char) - 2 > 0;       printf("_Alignof(type): %d\n", r);       hits += r;
    r = _Alignof(c) - 2 > 0;          printf("_Alignof(expr): %d\n", r);       hits += r;
    r = __alignof__(char) - 2 > 0;    printf("__alignof__(type): %d\n", r);    hits += r;
    r = __alignof__(c) - 2 > 0;       printf("__alignof__(expr): %d\n", r);    hits += r;
    r = __alignof(char) - 2 > 0;      printf("__alignof(type): %d\n", r);      hits += r;
    r = alignof(char) - 2 > 0;        printf("alignof(type): %d\n", r);        hits += r;
    r = alignof(c) - 2 > 0;           printf("alignof(expr): %d\n", r);        hits += r;
    r = -_Alignof(int) > 0;           printf("negated: %d\n", r);              hits += r;
    r = _Alignof(short) - 3 > 0;      printf("short minus 3: %d\n", r);        hits += r;
    return hits;
}
