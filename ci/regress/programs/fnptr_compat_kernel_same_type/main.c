/* The type checks the Linux kernel builds from __builtin_types_compatible_p,
 * applied to callbacks. */
#include <stdio.h>

#define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))
#define BUILD_BUG_ON_ZERO(e) ((int)(sizeof(struct { int:(-!!(e)); })))
#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))
#define typecheck_fn(type, function) ({ typeof(type) __tmp = function; (void)__tmp; })
#define must_match(a, b) BUILD_BUG_ON_ZERO(!__same_type(a, b))

struct file_ops {
    int (*open)(const char *path, int flags);
    void (*release)(int fd);
    long (*seek)(int fd, long off, int whence);
    int (*ioctl)(int fd, unsigned long cmd, ...);
};

static int my_open(const char *p, int f) { return p[0] + f; }
static void my_release(int fd) { printf("release %d\n", fd); }
static long my_seek(int fd, long off, int whence) { return fd + off + whence; }
static int my_ioctl(int fd, unsigned long cmd, ...) { return fd + (int)cmd; }
static int my_ioctl_fixed(int fd, unsigned long cmd) { return fd + (int)cmd; }
static long my_open_bad(const char *p, int f) { return p[0] + f; }

static struct file_ops ops = { my_open, my_release, my_seek, my_ioctl };
static int (*handlers[])(const char *, int) = { my_open, my_open, my_open };

int main(void)
{
    int score = 0;
    /* these would fail to compile if the types were reported as different */
    score += must_match(ops.open, &my_open);
    score += must_match(ops.release, &my_release);
    score += must_match(ops.seek, &my_seek);
    score += must_match(ops.ioctl, &my_ioctl);
    typecheck_fn(int (*)(const char *, int), my_open);
    typecheck_fn(typeof(ops.release), my_release);
    printf("array size %d\n", (int)ARRAY_SIZE(handlers));

    printf("open vs long-returning: %d\n", __same_type(ops.open, &my_open_bad));
    printf("ioctl vs fixed: %d\n", __same_type(ops.ioctl, &my_ioctl_fixed));
    printf("open vs release: %d\n", __same_type(ops.open, ops.release));
    printf("function vs pointer: %d\n", __same_type(my_open, &my_open));
    printf("call %d %ld\n", ops.open("a", 1), ops.seek(1, 2, 3));
    ops.release(7);
    return score + (int)ARRAY_SIZE(handlers);
}
