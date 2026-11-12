#include <stdio.h>
int gi = 5;
int garr[4] = {10, 20, 30, 40};
static int f(void) { return 7; }
int *p1 = {&gi};
int (*p2)(void) = {f};
int (*p3)(void) = {*f};
int (*p4)(void) = {{&f}};
void *p5 = {(void *)&gi};
int *p6 = {garr + 2};
int *p7 = {&garr[1]};
char *p8 = {0};
const char *p9 = {"lit"};
const char *p10 = {(const char *)"cast"};
int main(void) {
    static int *l1 = {&garr[3]};
    static const char *l2 = {"loc" + 1};
    printf("%d %d %d %d %d %d %d %d %s %s %d %s\n", *p1, p2(), p3(), p4(), *(int *)p5, *p6, *p7,
           p8 == 0, p9, p10, *l1, l2);
    return 0;
}
