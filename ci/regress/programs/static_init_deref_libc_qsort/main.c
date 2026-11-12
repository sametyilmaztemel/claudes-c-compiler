#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int (*cmp_t)(const void *, const void *);
static int cmpint(const void *a, const void *b) { return *(const int *)a - *(const int *)b; }
size_t (*len)(const char *) = *strlen;
int (*conv)(const char *) = **atoi;
void *(*mv)(void *, const void *, size_t) = &*memcpy;
cmp_t cmp = *cmpint;
struct ops { size_t (*len)(const char *); int (*conv)(const char *); } ops[] = {
    { *strlen, *atoi }, { &*strlen, **atoi },
};
int main(void) {
    int v[4] = {4, 2, 9, 1}, w[4];
    mv(w, v, sizeof v);
    qsort(w, 4, sizeof w[0], cmp);
    printf("%zu %d %d %d %d %d\n", len("hello"), conv("42"), w[0], w[3],
           (int)ops[0].len("ab") + ops[1].conv("5"), ops[1].len("xyz") == 3);
    return 0;
}
