# End-to-end program corpus

`ci/regress/programs.sh` compiles every program in this directory with the
built driver for x86-64, AArch64, RISC-V 64 and i686, runs it (under qemu for
the cross targets) and compares its output and exit status with the expected
files.

Layout: `<name>/main.c`, `<name>/expected.stdout` and `<name>/expected.ret`
(the exit status in decimal).

The expected files are what gcc produces. A program belongs here only if gcc
gives the same output and exit status on all four targets, so avoid output
that depends on `sizeof(long)`, the pointer width, the signedness of plain
`char` or the `long double` format.

Each group pins one fix. With the unpatched upstream compiler every program of
a group fails on every target, with two exceptions:
`compound_assign_shift_wide_count` passes on i686, which has no `__int128`,
and plain `char` was already signed on x86-64 and i686, so of the `char_*`
programs only `char_signed_char_distinct` fails on every target; the other
four fail on AArch64, and two of them on RISC-V as well.

| Programs | Fix | Upstream issue |
|---|---|---|
| `typedef_*` | A declarator name is in scope before its own initializer and for the later declarators of the same declaration (parser) | `anthropics/claudes-c-compiler#332`, row 5 |
| `hexfloat_*` | Hexadecimal floating constants are converted exactly and rounded once (lexer) | `anthropics/claudes-c-compiler#332`, row 6 |
| `static_init_*` | Address constants in static initializers are resolved instead of lowered to zero: `*f`, `&&label`, `&"s"` and braced string pointers (IR lowering) | `anthropics/claudes-c-compiler#332`, row 10 |
| `compound_assign_*` | A signed left operand of a compound assignment is converted by value when the common type is unsigned, as in `x /= sizeof(int)` (IR lowering) | none |
| `alignof_*` | `_Alignof` and `__alignof__` have type `size_t`, like `sizeof` (IR lowering) | `anthropics/claudes-c-compiler#332`, row 1 |
| `char_*` | Plain `char` follows the target (unsigned on AArch64 and RISC-V) and `-fsigned-char` / `-funsigned-char`; `signed char` is a distinct type | `anthropics/claudes-c-compiler#332`, row 2 |
