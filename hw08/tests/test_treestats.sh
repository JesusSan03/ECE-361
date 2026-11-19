#!/bin/sh
set -eu

program=${1:-./treestats}

actual=$(printf '27 14 35 10 19 31 42\n' | "$program")
if [ "$actual" != 'n 7, height 3' ]; then
    printf 'FAIL: seven numbers: %s\n' "$actual"
    exit 1
fi
echo 'PASS: treestats with seven numbers'

actual=$(printf '' | "$program")
if [ "$actual" != 'empty tree' ]; then
    printf 'FAIL: empty input: %s\n' "$actual"
    exit 1
fi
echo 'PASS: treestats with empty input'

set +e
actual=$(printf '1 2 3 4 nope\n' | "$program")
status=$?
set -e
if [ "$actual" != 'not a number after 4 values' ] || [ "$status" -ne 1 ]; then
    printf 'FAIL: invalid input: %s (exit %s)\n' "$actual" "$status"
    exit 1
fi
echo 'PASS: treestats with invalid input and exit 1'
