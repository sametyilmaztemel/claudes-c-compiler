#include <stdio.h>

/* Converting signed byte distances to element counts with a 64-bit element size. */
struct item { int id; int weight; short flags; short pad; };

static int items_between(const struct item *a, const struct item *b) {
    int bytes = (int)((const char *)a - (const char *)b);
    bytes /= (unsigned long long)sizeof(struct item);
    return bytes;
}

static int tail_bytes(int total, int used) {
    int left = used - total;
    left %= 4ULL;
    return left;
}

int main(void) {
    struct item table[16];
    int i, check = 0;
    for (i = 0; i < 16; i++) { table[i].id = i; table[i].weight = i * i; table[i].flags = 0; table[i].pad = 0; }
    printf("%d\n", items_between(&table[3], &table[11]));
    printf("%d\n", items_between(&table[11], &table[3]));
    printf("%d\n", items_between(&table[0], &table[15]));
    printf("%d\n", tail_bytes(10, 3));
    printf("%d\n", tail_bytes(3, 10));
    for (i = 0; i < 16; i += 5)
        check += items_between(&table[i], &table[15 - i]);
    printf("check = %d\n", check);
    return check < 0 ? 1 : 0;
}
