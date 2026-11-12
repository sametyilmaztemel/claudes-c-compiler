#!/usr/bin/env bash
# End-to-end regression checks for the include search path options: -nostdinc
# and the -I / -iquote / -isystem / -idirafter family, in separate and joined
# form. Each check compiles (and, where it says so, runs) a small program with
# the built driver for every target.
#
# Usage: ci/regress/include-paths.sh [bin-dir]   (default: target/release)
# Env:   REGRESS_REQUIRE_ALL=1  a target whose emulator or sysroot is missing
#                               makes the script fail instead of being skipped
#        REGRESS_VERBOSE=1      also print the checks that pass
# Exit:  non-zero if a check fails (or a target is BLOCKED with REGRESS_REQUIRE_ALL=1).
set -uo pipefail

bindir=$(cd "${1:-target/release}" 2>/dev/null && pwd) || { echo "bin dir not found: ${1:-target/release}" >&2; exit 2; }
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT
cd "$work" || exit 2

mkdir -p inc_i inc_sys inc_after inc_quote
echo '#define VAL_I 11'     > inc_i/hdr_i.h
echo '#define VAL_SYS 22'   > inc_sys/hdr_sys.h
echo '#define VAL_AFTER 33' > inc_after/hdr_after.h
echo '#define VAL_QUOTE 44' > inc_quote/hdr_quote.h
echo '#define VAL_LOCAL 55' > hdr_local.h
printf '#include <stdio.h>\nint main(void){printf("hello\\n");return 0;}\n' > sys_stdio.c
printf '#include <stddef.h>\nsize_t x;int main(void){return 0;}\n' > sys_stddef.c
printf '#include <hdr_i.h>\n#include <hdr_sys.h>\n#include <hdr_after.h>\n#include "hdr_quote.h"\n#include "hdr_local.h"\nint main(void){return (VAL_I+VAL_SYS+VAL_AFTER+VAL_QUOTE+VAL_LOCAL)==165 ? 0 : 1;}\n' > explicit.c
printf 'int add(int a,int b){return a+b;}\nint main(void){return add(40,2)==42 ? 0 : 1;}\n' > freestanding.c
printf '#if __has_include(<stdio.h>)\nint main(void){return 1;}\n#else\nint main(void){return 0;}\n#endif\n' > hasinc.c
printf 'int main(void){return 0;}\n' > plain.c
printf '#include <stdio.h>\n.globl sym\nsym:\n' > sysinc.S
printf '#include "hdr_local.h"\n.globl sym\n.data\nsym:\n.long VAL_LOCAL\n' > localinc.S
printf '#include <%s/hdr_local.h>\n#if !__has_include(<%s/hdr_local.h>)\n#error absolute path not found\n#endif\nint main(void){return VAL_LOCAL==55 ? 0 : 1;}\n' "$work" "$work" > absolute.c
printf '#include <hdr_sys.h>\nint main(void){return VAL_SYS==22 ? 0 : 1;}\n' > angle_sys.c
printf '#include <hdr_after.h>\nint main(void){return VAL_AFTER==33 ? 0 : 1;}\n' > angle_after.c
printf '#include "hdr_quote.h"\nint main(void){return VAL_QUOTE==44 ? 0 : 1;}\n' > quote_only.c
printf '#include <hdr_quote.h>\nint main(void){return 0;}\n' > angle_quote.c

# name | binary | runner (empty = native) | sysroot | header that only the bundled directory has
targets=(
  "x86-64|ccc|||emmintrin.h"
  "aarch64|ccc-arm|qemu-aarch64|/usr/aarch64-linux-gnu|arm_neon.h"
  "riscv64|ccc-riscv|qemu-riscv64|/usr/riscv64-linux-gnu|"
  "i686|ccc-i686|qemu-i386|/usr/i686-linux-gnu|emmintrin.h"
)

pass=0; fail=0; blocked=0

check() { # description, expected, actual
  if [ "$2" = "$3" ]; then
    pass=$((pass + 1))
    [ "${REGRESS_VERBOSE:-0}" = "1" ] && printf '  ok   %s\n' "$1"
  else
    fail=$((fail + 1))
    printf '  FAIL %s (expected %s, got %s)\n' "$1" "$2" "$3"
  fi
  return 0
}
rc() { "$@" >/dev/null 2>stderr.txt; echo $?; }

for t in "${targets[@]}"; do
  IFS='|' read -r name bin runner sysroot bundled <<<"$t"
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
  run() { if [ -n "$runner" ]; then "$runner" -L "$sysroot" "$@"; else "$@"; fi; }
  before=$fail

  # Default mode is unaffected.
  check "$name: default: hello world compiles"              0 "$(rc "$cc" -o hello sys_stdio.c)"
  check "$name: default: hello world runs"                  0 "$(rc run ./hello)"

  # -nostdinc hides every default directory.
  check "$name: -nostdinc <stdio.h> rejected (-c)"          1 "$(rc "$cc" -nostdinc -c -o o1.o sys_stdio.c)"
  check "$name:   and the diagnostic names the header"      1 "$(grep -c 'stdio.h: No such file or directory' stderr.txt)"
  check "$name: -nostdinc <stddef.h> rejected (-S)"         1 "$(rc "$cc" -nostdinc -S -o o2.s sys_stddef.c)"
  check "$name: -nostdinc <stdio.h> rejected (link)"        1 "$(rc "$cc" -nostdinc -o o3 sys_stdio.c)"
  check "$name: -nostdinc -include stdio.h rejected"        1 "$(rc "$cc" -nostdinc -include stdio.h -c -o o4.o plain.c)"
  check "$name: -nostdinc __has_include(<stdio.h>) is 0"    0 "$(rc "$cc" -nostdinc -o hasinc hasinc.c)"
  check "$name:   and the program returns 0"                0 "$(rc run ./hasinc)"
  "$cc" -E sysinc.S > s_default.out 2>/dev/null
  check "$name: default: .S finds <stdio.h> (-E)"           1 "$(grep -c 'stdio.h"' s_default.out | sed 's/^[1-9][0-9]*$/1/')"
  "$cc" -nostdinc -E sysinc.S > s_nostdinc.out 2>/dev/null
  check "$name: -nostdinc .S does not search <stdio.h>"     0 "$(grep -c 'stdio.h"' s_nostdinc.out)"
  if [ -n "$bundled" ]; then
    printf '#include <%s>\nint main(void){return 0;}\n' "$bundled" > bundled.c
    check "$name: default: bundled <$bundled> found (-E)"   0 "$(rc "$cc" -E bundled.c)"
    check "$name: -nostdinc bundled <$bundled> rejected"    1 "$(rc "$cc" -nostdinc -E bundled.c)"
    check "$name: -nostdinc -isystem \$(print-file-name=include) finds it" \
                                                              0 "$(rc "$cc" -nostdinc -isystem "$("$cc" -print-file-name=include)" -E bundled.c)"
  fi

  # Explicitly named directories, local files and absolute paths still work.
  check "$name: -nostdinc -I -isystem -idirafter -iquote"   0 "$(rc "$cc" -nostdinc -Iinc_i -isystem inc_sys -idirafter inc_after -iquote inc_quote -o explicit explicit.c)"
  check "$name:   and the program returns 0"                0 "$(rc run ./explicit)"
  check "$name: -nostdinc absolute <path> include"          0 "$(rc "$cc" -nostdinc -o absolute absolute.c)"
  check "$name:   and the program returns 0"                0 "$(rc run ./absolute)"
  check "$name: -nostdinc source without includes"          0 "$(rc "$cc" -nostdinc -o freestanding freestanding.c)"
  check "$name:   and the program returns 0"                0 "$(rc run ./freestanding)"
  check "$name: -nostdinc .S with a local include"          0 "$(rc "$cc" -nostdinc -c -o o6.o localinc.S)"

  # The directory may be joined to -isystem, -idirafter and -iquote.
  check "$name: -isysteminc_sys (joined)"                   0 "$(rc "$cc" -isysteminc_sys -o j1 angle_sys.c)"
  check "$name:   and the program returns 0"                0 "$(rc run ./j1)"
  check "$name: -idirafterinc_after (joined)"               0 "$(rc "$cc" -idirafterinc_after -o j2 angle_after.c)"
  check "$name: -iquoteinc_quote (joined), quoted include"  0 "$(rc "$cc" -iquoteinc_quote -o j3 quote_only.c)"
  check "$name: -iquoteinc_quote does not serve <...>"      1 "$(rc "$cc" -iquoteinc_quote -c -o j4.o angle_quote.c)"
  check "$name: -nostdinc -isysteminc_sys (joined)"         0 "$(rc "$cc" -nostdinc -isysteminc_sys -o j5 angle_sys.c)"

  if [ "$fail" -eq "$before" ]; then echo "$name: PASS"; else echo "$name: FAIL ($((fail - before)) check(s))"; fi
done

echo "include-paths: $pass passed, $fail failed, $blocked target(s) blocked"
if [ "$fail" -gt 0 ]; then exit 1; fi
if [ "$blocked" -gt 0 ] && [ "${REGRESS_REQUIRE_ALL:-0}" = "1" ]; then exit 1; fi
exit 0
