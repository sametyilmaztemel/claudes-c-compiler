#include <stdio.h>
struct T { char (*whole)[6]; const char *mid; int k; };
struct T t1 = { &"Hello", &"World"[2], 3 };
struct T tarr[2] = { { &"abcde", &"vwxyz"[4], 1 }, { .mid = &"12345"[1], .whole = &"67890" } };
char (*ptrs[2])[4] = { &"abc", &"xyz" };
union U { char (*w)[3]; long l; } u = { &"hi" };
int main(void){
    printf("%s %s %d %s %s %s %s %s %s %s\n", *t1.whole, t1.mid, t1.k, *tarr[0].whole, tarr[0].mid, *tarr[1].whole, tarr[1].mid, *ptrs[0], *ptrs[1], *u.w);
    return 0; }
