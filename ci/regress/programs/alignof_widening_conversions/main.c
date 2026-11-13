/* Converting a wrapped _Alignof difference to a wider or floating type must
   zero-extend (or convert from unsigned), not sign-extend. On 32-bit targets
   size_t is 32 bits wide, so widening to 64 bits is where it shows. */
#include <stdio.h>
#include <stddef.h>

static unsigned long long to_ull(void) { return _Alignof(char) - 2; }

int main(void) {
    int hits = 0;
    int r;
    unsigned long long wrapped = (unsigned long long)(_Alignof(char) - 2);
    unsigned long long expect = (unsigned long long)(size_t)-1;

    r = wrapped == expect;                       printf("cast: %d\n", r);          hits += r;
    r = to_ull() == expect;                      printf("return: %d\n", r);        hits += r;
    r = (double)(_Alignof(char) - 2) > 0;        printf("to double: %d\n", r);     hits += r;
    r = (float)(_Alignof(char) - 2) > 1000.0f;   printf("to float: %d\n", r);      hits += r;
    r = (wrapped >> 31) != 0;                    printf("high bits: %d\n", r);     hits += r;
    return hits;
}
