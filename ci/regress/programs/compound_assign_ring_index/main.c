#include <stdio.h>

/* A ring buffer addressed by signed offsets: the wrap-around uses an unsigned
   64-bit modulus, so the offset must keep its sign when it is converted. */
#define CAP 7ULL

static int slots[7];

static int wrap(int offset) {
    offset %= CAP;
    return offset;
}

static int step_back(int pos, int n) {
    int back = -n;
    back %= CAP;
    pos += back;
    return pos;
}

int main(void) {
    int i, sum = 0;
    for (i = 0; i < 7; i++) slots[i] = (i + 1) * 11;
    for (i = -10; i <= 10; i += 5) {
        int w = wrap(i);
        printf("wrap(%d) = %d\n", i, w);
        sum += w;
    }
    printf("step_back(3, 5) = %d\n", step_back(3, 5));
    printf("step_back(0, 20) = %d\n", step_back(0, 20));
    printf("sum = %d\n", sum);
    return sum & 0x7f;
}
