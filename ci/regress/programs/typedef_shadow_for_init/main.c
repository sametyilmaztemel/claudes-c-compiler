/* A for-init declaration list whose later declarators use an earlier one that
 * reuses a typedef name.  (The typedef is not used again in this function:
 * a for-init shadow outliving the loop is a separate, known limitation.) */
#include <stdio.h>

typedef char x;

int main(void) {
    int s = 0;
    for (int i = 0, x = 3, n = sizeof(x); i < n; i++)
        s += x;
    printf("%d\n", s);
    return s;
}
