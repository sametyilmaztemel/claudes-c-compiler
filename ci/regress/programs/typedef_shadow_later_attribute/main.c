#include <stdio.h>

typedef char x[16];
typedef char z[16];

int main(void)
{
    /* The attribute of y is parsed after the variable x is in scope, so
       sizeof(x) there is sizeof(int); the attribute of z is part of the
       declarator that declares z, so sizeof(z) there is still the typedef. */
    int x = 0, y __attribute__((aligned(sizeof(x)))) = 0;
    int z __attribute__((aligned(sizeof(z)))) = 0;
    int ay = (int)__alignof__(y);
    int az = (int)__alignof__(z);
    printf("%d %d\n", ay, az);
    return ay + az + x + y + z;
}
