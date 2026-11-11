#!/usr/bin/env bash
# Cross-target smoke test: compile, run and check a small C program per target.
# Usage: ci/smoke.sh [bin-dir]   (default: target/release)
# Env:   SMOKE_REQUIRE_ALL=1  -> BLOCKED targets make the script fail.
# Exit:  non-zero if any target FAILs (or is BLOCKED with SMOKE_REQUIRE_ALL=1).
set -uo pipefail

bindir="${1:-target/release}"
work="$(mktemp -d)"
trap 'rm -rf "$work"' EXIT

cat > "$work/smoke.c" <<'CEOF'
#include <stdio.h>

struct point { int x; int y; };

static int add(int a, int b) { return a + b; }
static int mul(int a, int b) { return a * b; }

int main(void) {
    int (*op)(int, int) = add;
    struct point p = { 3, 4 };
    int sum = 0;
    for (int i = 1; i <= 10; i++)
        sum += i * i;
    printf("sum=%d\n", sum);
    printf("point=%d,%d\n", p.x, p.y);
    printf("add=%d\n", op(p.x, p.y));
    op = mul;
    printf("mul=%d\n", op(p.x, p.y));
    double d = 1.5 * 4.0 + 0.25;
    printf("fp=%.2f\n", d);
    long long big = 1000000LL * 3000LL;
    printf("big=%lld\n", big);
    printf("div=%d mod=%d\n", -17 / 5, -17 % 5);
    return 42;
}
CEOF

expected_out='sum=385
point=3,4
add=7
mul=12
fp=6.25
big=3000000000
div=-3 mod=-2'
expected_rc=42

# name | binary | runner (empty = native) | sysroot dir (empty = none)
targets=(
  "x86-64|ccc|||"
  "aarch64|ccc-arm|qemu-aarch64|/usr/aarch64-linux-gnu"
  "riscv64|ccc-riscv|qemu-riscv64|/usr/riscv64-linux-gnu"
  "i686|ccc-i686|qemu-i386|/usr/i686-linux-gnu"
)

fail=0
blocked=0

for t in "${targets[@]}"; do
  IFS='|' read -r name bin runner sysroot _ <<<"$t"
  exe="$work/smoke-$name"

  if [ -n "$runner" ]; then
    if ! command -v "$runner" >/dev/null 2>&1; then
      echo "$name: BLOCKED ($runner not installed)"; blocked=$((blocked + 1)); continue
    fi
    if [ ! -d "$sysroot" ]; then
      echo "$name: BLOCKED (sysroot $sysroot not found)"; blocked=$((blocked + 1)); continue
    fi
  fi

  if [ ! -x "$bindir/$bin" ]; then
    echo "$name: FAIL ($bindir/$bin not found or not executable)"; fail=$((fail + 1)); continue
  fi

  if ! "$bindir/$bin" -o "$exe" "$work/smoke.c" >"$work/$name.cc.log" 2>&1; then
    echo "$name: FAIL (compile error)"; sed 's/^/    /' "$work/$name.cc.log" | head -20
    fail=$((fail + 1)); continue
  fi

  if [ -n "$runner" ]; then
    out="$(timeout 60 "$runner" -L "$sysroot" "$exe" 2>"$work/$name.err")"; rc=$?
  else
    out="$(timeout 60 "$exe" 2>"$work/$name.err")"; rc=$?
  fi

  if [ "$out" = "$expected_out" ] && [ "$rc" -eq "$expected_rc" ]; then
    echo "$name: PASS"
  else
    echo "$name: FAIL (exit=$rc, expected $expected_rc; stdout mismatch: $([ "$out" = "$expected_out" ] && echo no || echo yes))"
    printf '%s\n' "$out" | sed 's/^/    got: /'
    fail=$((fail + 1))
  fi
done

if [ "$fail" -gt 0 ]; then
  echo "smoke: $fail target(s) FAILED"; exit 1
fi
if [ "$blocked" -gt 0 ]; then
  echo "smoke: $blocked target(s) BLOCKED (not verified)"
  [ "${SMOKE_REQUIRE_ALL:-0}" = "1" ] && exit 1
fi
exit 0
