# ECE 361 — Homework 3: Pointers

This folder implements four pointer functions: `swap_float`, `minmax`,
`find_above`, and `reverse`. In `ptr.c`, arrays are traversed using pointer
arithmetic instead of indexing.

## Build and test

From the `hw03` directory:

```sh
make
make test
make clean
```

`make` builds the test executable. `make test` runs all tests and prints
PASS/FAIL per case and a summary. A failing test makes the program exit with
nonzero status. `make clean` removes generated object and executable files.

Compilation uses `gcc -std=c11 -Wall -Wextra`.
