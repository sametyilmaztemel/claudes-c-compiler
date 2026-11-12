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

Each group pins one fix. Without that fix every program of the group fails on
every target.

| Programs | Fix | Upstream issue |
|---|---|---|
| `typedef_*` | A declarator name is in scope before its own initializer and for the later declarators of the same declaration (parser) | `anthropics/claudes-c-compiler#332`, row 5 |
| `hexfloat_*` | Hexadecimal floating constants are converted exactly and rounded once (lexer) | `anthropics/claudes-c-compiler#332`, row 6 |
