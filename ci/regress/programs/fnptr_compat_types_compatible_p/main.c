/* __builtin_types_compatible_p on function pointer types: the parameter
 * list of each operand is part of the type. */
#include <stdio.h>

typedef int fn_float_t(float);
typedef int (*pfn_float_t)(float);
typedef pfn_float_t *ppfn_float_t;
static int handler(float f) { return (int)f; }
static long other(float f) { return (long)f; }

static int failures;

#define CHECK(a, b, want) do { \
        int got = __builtin_types_compatible_p(a, b); \
        printf("%-3s %-34s | %s\n", got == (want) ? "ok" : "BAD", #a, #b); \
        if (got != (want)) failures++; \
    } while (0)

int main(void)
{
    CHECK(int(*)(float, double), int(*)(float), 0);
    CHECK(int(*)(float), int(*)(float), 1);
    CHECK(int(*)(float), long(*)(float), 0);
    CHECK(int(*)(float), int(*)(double), 0);
    CHECK(int(*)(int, ...), int(*)(int), 0);
    CHECK(int(*)(int, ...), int(*)(int, ...), 1);
    CHECK(int(*)(int x), int(*)(int y), 1);
    CHECK(int(*)(const int), int(*)(int), 1);
    CHECK(int(*)(int[]), int(*)(int *), 1);
    CHECK(int(*)(int(int)), int(*)(int(*)(int)), 1);
    CHECK(int(**)(int), int(**)(long), 0);
    CHECK(int(*)(float), int *, 0);
    CHECK(int(*)[3], int *[3], 0);
    CHECK(int(*)[3], int(*)[3], 1);
    CHECK(fn_float_t *, pfn_float_t, 1);
    CHECK(fn_float_t *, int(*)(float), 1);
    CHECK(ppfn_float_t, int(**)(float), 1);
    CHECK(ppfn_float_t, int(**)(double), 0);
    CHECK(typeof(handler) *, pfn_float_t, 1);
    CHECK(typeof(&handler), typeof(&other), 0);
    CHECK(int *, int *, 1);
    CHECK(int *, long *, 0);
    printf("failures %d\n", failures);
    return failures;
}
