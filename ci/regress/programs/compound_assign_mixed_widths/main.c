#include <stdio.h>

/* Every narrow signed type against unsigned int and unsigned long long operands. */
static long long vi(long long x) { return x; }

int main(void) {
    signed char c;
    short s;
    int i;
    int acc = 0;

    c = (signed char)vi(-100); c /= 7u;   printf("c /= 7u    -> %d\n", c);   acc += c;
    c = (signed char)vi(-100); c %= 7u;   printf("c %%= 7u    -> %d\n", c);  acc += c;
    c = (signed char)vi(-100); c /= 7ULL; printf("c /= 7ULL  -> %d\n", c);   acc += c;
    c = (signed char)vi(-100); c %= 7ULL; printf("c %%= 7ULL  -> %d\n", c);  acc += c;
    s = (short)vi(-30001); s /= 11u;      printf("s /= 11u   -> %d\n", s);   acc += s & 0xff;
    s = (short)vi(-30001); s %= 11u;      printf("s %%= 11u   -> %d\n", s);  acc += s;
    s = (short)vi(-30001); s /= 11ULL;    printf("s /= 11ULL -> %d\n", s);   acc += s & 0xff;
    s = (short)vi(-30001); s %= 11ULL;    printf("s %%= 11ULL -> %d\n", s);  acc += s;
    i = (int)vi(-2000000011); i /= 11ULL; printf("i /= 11ULL -> %d\n", i);   acc += i & 0xff;
    i = (int)vi(-2000000011); i %= 11ULL; printf("i %%= 11ULL -> %d\n", i);  acc += i;
    i = (int)vi(-2000000011); i /= 11u;   printf("i /= 11u   -> %d\n", i);   acc += i & 0xff;
    i = (int)vi(-2000000011); i %= 11u;   printf("i %%= 11u   -> %d\n", i);  acc += i;
    printf("acc = %d\n", acc);
    return acc & 0x3f;
}
