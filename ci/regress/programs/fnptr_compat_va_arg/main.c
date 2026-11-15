/* __builtin_va_arg with function pointer and pointer-to-array operands. */
#include <stdarg.h>
#include <stdio.h>

typedef int (*op_t)(int);
typedef int row_t[4];

static int twice(int x) { return 2 * x; }
static int negate(int x) { return -x; }
static double halve(double x) { return x / 2; }
static op_t select_op(int which) { return which ? twice : negate; }

static int run(int n, ...)
{
    va_list ap;
    va_start(ap, n);
    int (*f)(int) = va_arg(ap, int (*)(int));
    double (*g)(double) = va_arg(ap, double (*)(double));
    op_t h = va_arg(ap, op_t);
    op_t (*sel)(int) = va_arg(ap, op_t (*)(int));
    int (**slot)(int) = va_arg(ap, int (**)(int));
    int (*arr)[3] = va_arg(ap, int (*)[3]);
    row_t *row = va_arg(ap, row_t *);
    int (*(*nested)(int))(int) = va_arg(ap, int (*(*)(int))(int));
    va_end(ap);

    printf("f %d g %.2f h %d\n", f(n), g(n), h(n + 1));
    printf("sel %d slot %d nested %d\n", sel(0)(5), (*slot)(6), nested(1)(7));
    printf("arr %d %d %d row %d\n", (*arr)[0], arr[0][1], (*arr)[2], (*row)[3]);
    printf("sizes %d %d\n", (int)sizeof(*va_arg(ap, int (*)[3])) == 3 * (int)sizeof(int),
           (int)sizeof(*va_arg(ap, row_t *)) == 4 * (int)sizeof(int));
    return (*arr)[1] + (*row)[3];
}

int main(void)
{
    int a[3] = { 1, 2, 3 };
    row_t r = { 0, 0, 0, 9 };
    op_t slot = negate;
    int rc = run(4, twice, halve, negate, select_op, &slot, &a, &r, select_op);
    printf("rc %d\n", rc);
    return rc;
}
