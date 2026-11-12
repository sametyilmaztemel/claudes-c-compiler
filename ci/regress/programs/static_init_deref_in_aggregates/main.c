#include <stdio.h>
static int f(void){ return 4; }
static int g(void){ return 6; }
struct H { int (*fp)(void); };
struct H h1 = { *f };
struct H harr[] = { { *f }, { &*f }, { (int (*)(void))*&f }, { **g } };
int (*pp[3])(void) = { [1] = *f, [2] = **g };
int (*tab[])(void) = { *f, &*g, *&f, ***g };
static int (**ppp)(void) = &pp[1];
int main(void){
    static int (*sl)(void) = *g;
    static struct H sh = { **f };
    printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", h1.fp(), harr[0].fp(), harr[1].fp(), harr[2].fp(), harr[3].fp(),
           pp[1](), pp[2](), tab[0](), tab[1](), tab[2](), tab[3](), (*ppp)(), sl(), sh.fp());
    return 0; }
