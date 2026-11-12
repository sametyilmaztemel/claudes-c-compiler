#include <stdio.h>
char (*c)[] = &"Hello";
int main(void){ puts(*c); return 0; }
