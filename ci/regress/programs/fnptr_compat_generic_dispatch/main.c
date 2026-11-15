/* _Generic selection on function types: the parameter names in the
 * declarations must not take part in the comparison. */
#include <stdio.h>

static void log_event(int level) { printf("event %d\n", level); }
static int add_one(int value) { return value + 1; }
static long widen(int value, int scale) { return (long)value * scale; }
static double mean(double first, double second) { return (first + second) / 2; }
static char *ident(char *text) { return text; }
static int count_args(const char *fmt, ...) { return fmt[0] == '%' ? 1 : 0; }

#define KIND(f) _Generic((f), \
    int(*)(int): "int(int)", \
    void(*)(int): "void(int)", \
    long(*)(int, int): "long(int,int)", \
    double(*)(double, double): "double(double,double)", \
    int(*)(const char *, ...): "int(const char*,...)", \
    default: "other")

#define CODE(f) _Generic((f), \
    void(*)(int): 1, int(*)(int): 2, long(*)(int, int): 4, \
    double(*)(double, double): 8, int(*)(const char *, ...): 16, default: 32)

#define INVOKE(f, a) _Generic((f), \
    void(*)(int): (log_event(a), 0), \
    int(*)(int): add_one(a), \
    default: -1)

int main(void)
{
    void (*handler)(int) = log_event;
    long (*scaler)(int, int) = widen;
    int mask = 0;

    puts(KIND(log_event));
    puts(KIND(add_one));
    puts(KIND(widen));
    puts(KIND(mean));
    puts(KIND(count_args));
    puts(KIND(&add_one));
    puts(KIND(handler));
    puts(KIND(scaler));
    puts(KIND(ident));

    mask |= CODE(log_event) | CODE(add_one) | CODE(widen) | CODE(mean) | CODE(count_args);
    printf("mask %d\n", mask);
    printf("invoke %d %d\n", INVOKE(log_event, 3), INVOKE(add_one, 41));
    printf("invoke %d\n", INVOKE(widen, 1));
    return mask;
}
