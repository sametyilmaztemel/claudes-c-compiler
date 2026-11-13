/* Cases that must keep their meaning: explicit signed casts, conversions to
   signed destinations, sizeof, and values that never wrap. The last check
   is the bug itself, so the program does not pass on a compiler that types
   _Alignof as a signed int. */
#include <stdio.h>
#include <stddef.h>

int main(void) {
    int k = _Alignof(char) - 2;        /* converted to int: -1 */
    long m = (long)_Alignof(char) - 2; /* explicit signed cast: -1 */
    int hits = 0;
    int r;

    printf("int destination: %d\n", k);
    r = m > 0;                              printf("long cast: %d\n", r);          hits += r;
    r = (int)_Alignof(char) - 2 > 0;        printf("int cast: %d\n", r);           hits += r;
    r = _Alignof(int) == 4;                 printf("plain equality: %d\n", r);     hits += r;
    r = _Alignof(int) * 2 == 8;             printf("no wrap: %d\n", r);            hits += r;
    r = sizeof(char) - 2 > 0;               printf("sizeof wraps: %d\n", r);       hits += r;
    r = -1 > sizeof(int);                   printf("sizeof vs -1: %d\n", r);       hits += r;
    r = _Alignof(char) - 2 > 0;             printf("alignof wraps: %d\n", r);      hits += r;
    return hits;
}
