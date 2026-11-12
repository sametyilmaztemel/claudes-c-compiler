#include <stdio.h>
#include <string.h>
char (*a)[4] = &"abc";
const char (*b)[2] = &"x";
char (*arr[])[3] = { &"ab", &"cd", &"ef" };
struct S { char (*p)[4]; const char *q; int k; } s[] = {
    { &"one", &"two"[1], 1 },
    { .q = &"three"[2], .k = 2, .p = &"fou" },
};
static const char *q = &"quux"[0];
int main(void) {
    printf("%s %s %s %s %s\n", *a, *b, *arr[0], *arr[2], q);
    printf("%s %s %d %s %s %d\n", *s[0].p, s[0].q, s[0].k, *s[1].p, s[1].q, s[1].k);
    printf("%zu %zu\n", sizeof *a, strlen(*arr[1]));
    return 0;
}
