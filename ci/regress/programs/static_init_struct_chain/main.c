#include <stdio.h>
struct Node { const char *name; int (*fn)(void); struct Node *next; };
static int one(void) { return 1; }
static int two(void) { return 2; }
static struct Node n2 = { "two", *two, 0 };
static struct Node n1 = { &"one"[0], **one, &n2 };
int main(void) {
    int sum = 0;
    for (struct Node *n = &n1; n; n = n->next) { printf("%s ", n->name); sum += n->fn(); }
    printf("%d\n", sum);
    return 0;
}
