#include <stdio.h>
int (*g)(const char*) = *puts;
int main(void){ g("3"); return 0; }
