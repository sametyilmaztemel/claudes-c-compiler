#!/usr/bin/env bash
# End-to-end program corpus. Every directory under ci/regress/programs/ holds
#   main.c            the program
#   expected.stdout   what it must print
#   expected.ret      its exit status (decimal)
# The expected files are what gcc produces, and gcc produces the same on all
# four targets. Each program is compiled with the built driver for every
# target, run (under qemu for the cross targets) and compared.
#
# Usage: ci/regress/programs.sh [bin-dir]   (default: target/release)
# Env:   REGRESS_REQUIRE_ALL=1  a target whose emulator or sysroot is missing
#                               makes the script fail instead of being skipped
#        REGRESS_VERBOSE=1      also print the programs that pass
# Exit:  non-zero if a program fails (or a target is BLOCKED with REGRESS_REQUIRE_ALL=1).
set -uo pipefail

here=$(cd "$(dirname "$0")" && pwd)
bindir=$(cd "${1:-target/release}" 2>/dev/null && pwd) || { echo "bin dir not found: ${1:-target/release}" >&2; exit 2; }
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT

# name | binary | runner (empty = native) | sysroot
targets=(
  "x86-64|ccc||"
  "aarch64|ccc-arm|qemu-aarch64|/usr/aarch64-linux-gnu"
  "riscv64|ccc-riscv|qemu-riscv64|/usr/riscv64-linux-gnu"
  "i686|ccc-i686|qemu-i386|/usr/i686-linux-gnu"
)

pass=0; fail=0; blocked=0; programs=0

for t in "${targets[@]}"; do
  IFS='|' read -r name bin runner sysroot <<<"$t"
  cc="$bindir/$bin"

  if [ -n "$runner" ]; then
    if ! command -v "$runner" >/dev/null 2>&1; then
      echo "$name: BLOCKED ($runner not installed)"; blocked=$((blocked + 1)); continue
    fi
    if [ ! -d "$sysroot" ]; then
      echo "$name: BLOCKED (sysroot $sysroot not found)"; blocked=$((blocked + 1)); continue
    fi
  fi
  if [ ! -x "$cc" ]; then
    echo "$name: FAIL ($cc not found or not executable)"; fail=$((fail + 1)); continue
  fi
  before=$fail
  programs=0

  for dir in "$here"/programs/*/; do
    [ -f "$dir/main.c" ] || continue
    prog=$(basename "$dir")
    programs=$((programs + 1))
    want_out=$(cat "$dir/expected.stdout")
    want_rc=$(cat "$dir/expected.ret")
    exe="$work/$name-$prog"
    if ! "$cc" -o "$exe" "$dir/main.c" >"$work/cc.log" 2>&1; then
      fail=$((fail + 1))
      printf '  FAIL %s: %s does not compile: %s\n' "$name" "$prog" "$(head -n 1 "$work/cc.log")"
      continue
    fi
    if [ -n "$runner" ]; then cmd=(timeout 60 "$runner" -L "$sysroot" "$exe"); else cmd=(timeout 60 "$exe"); fi
    got_out=$("${cmd[@]}" 2>/dev/null); got_rc=$?
    if [ "$got_out" = "$want_out" ] && [ "$got_rc" = "$want_rc" ]; then
      pass=$((pass + 1))
      [ "${REGRESS_VERBOSE:-0}" = "1" ] && printf '  ok   %s: %s\n' "$name" "$prog"
    elif [ "$got_out" = "$want_out" ]; then
      fail=$((fail + 1))
      printf '  FAIL %s: %s exits with %s (expected %s)\n' "$name" "$prog" "$got_rc" "$want_rc"
    else
      fail=$((fail + 1))
      printf '  FAIL %s: %s prints the wrong output (exit status %s, expected %s)\n' "$name" "$prog" "$got_rc" "$want_rc"
    fi
  done

  if [ "$fail" -eq "$before" ]; then echo "$name: PASS ($programs programs)"; else echo "$name: FAIL ($((fail - before)) of $programs programs)"; fi
done

echo "programs: $pass passed, $fail failed, $blocked target(s) blocked"
if [ "$fail" -gt 0 ]; then exit 1; fi
if [ "$programs" -eq 0 ] && [ "$blocked" -eq 0 ]; then echo "no programs found under $here/programs" >&2; exit 1; fi
if [ "$blocked" -gt 0 ] && [ "${REGRESS_REQUIRE_ALL:-0}" = "1" ]; then exit 1; fi
exit 0
