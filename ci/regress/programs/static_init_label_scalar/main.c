#include <stdio.h>
int main(void){ static void *p = &&L; goto *p; puts("bad"); L: puts("ok"); return 0; }
