/* Function pointer type comparisons in constant expressions. */
#include <stdio.h>

#define SAME(a, b) __builtin_types_compatible_p(a, b)

enum {
    DIFFERENT_COUNT = SAME(int(*)(float, double), int(*)(float)),
    SAME_SIGNATURE = SAME(int(*)(float), int(*)(float)),
    DIFFERENT_RETURN = SAME(int(*)(int), long(*)(int)),
    VARIADIC = SAME(void(*)(int, ...), void(*)(int)),
    NAMES_IGNORED = SAME(int(*)(int a), int(*)(int b)),
    NESTED = SAME(int(*(*)(int))(float), int(*(*)(int))(double)),
};

static char count_table[1 + SAME(int(*)(int), int(*)(int)) * 3 + SAME(int(*)(int), int(*)(long)) * 7];
static const int in_initializer = SAME(void(*)(char), void(*)(char));
static int table[] = { SAME(int(**)(int), int(**)(int)), SAME(int(**)(int), int(**)(char)) };

_Static_assert(!SAME(int(*)(float, double), int(*)(float)), "count");
_Static_assert(SAME(int(*)(int x), int(*)(int)), "names");
_Static_assert(!SAME(void(*)(int, ...), void(*)(int)), "variadic");

static int classify(int v)
{
    switch (v) {
    case SAME(int(*)(int), long(*)(int)): return 10;
    case 1 + SAME(int(*)(int), int(*)(int)): return 20;
    default: return -1;
    }
}

int main(void)
{
    printf("%d %d %d %d %d %d\n", DIFFERENT_COUNT, SAME_SIGNATURE, DIFFERENT_RETURN, VARIADIC, NAMES_IGNORED, NESTED);
    printf("%d %d\n", (int)sizeof count_table, in_initializer);
    printf("%d %d\n", table[0], table[1]);
    printf("%d %d %d\n", classify(0), classify(2), classify(5));
    return (int)sizeof count_table;
}
