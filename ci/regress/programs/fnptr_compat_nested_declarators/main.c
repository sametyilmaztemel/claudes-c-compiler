/* Nested abstract declarators in the type operands, and the functions they
 * describe. */
#include <stdio.h>

typedef int (*unary_t)(float);

static int truncate_to_int(float f) { return (int)f; }
static int round_up(float f) { int i = (int)f; return i < f ? i + 1 : i; }
static unary_t pick(int round) { return round ? round_up : truncate_to_int; }
static int (*(*get_picker(void))(int))(float) { return pick; }

#define T(a, b) printf("%-36s | %-36s %d\n", #a, #b, __builtin_types_compatible_p(a, b))

int main(void)
{
    T(int(*(*)(int))(float), int(*(*)(int))(float));
    T(int(*(*)(int))(float), int(*(*)(int))(double));
    T(int(*(*)(int))(float), int(*(*)(long))(float));
    T(int(*(*)(int))(float), long(*(*)(int))(float));
    T(unary_t(*)(int), int(*(*)(int))(float));
    T(unary_t(*)(int), int(*(*)(int))(double));
    T(int(*(**)(int))(float), int(*(**)(int))(float));
    T(void(*(*)(void))(void), void(*(*)(void))(void));
    T(char *(*(*)(int, ...))(float), char *(*(*)(int))(float));
    T(typeof(&pick), int(*(*)(int))(float));

    printf("%d %d\n", pick(0)(2.5f), pick(1)(2.5f));
    printf("%d\n", get_picker()(1)(7.25f));
    return get_picker()(1)(7.25f);
}
