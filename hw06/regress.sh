#!/bin/sh
set -eu
for f in scenarios/*.txt; do
    name=$(basename "$f")
    ./replay "$f" > "${TMPDIR:-/tmp}/hw06_$$.trace"
    if diff -u "expected/$name" "${TMPDIR:-/tmp}/hw06_$$.trace"; then
        echo "PASS regression: $name"
    else
        rm -f "${TMPDIR:-/tmp}/hw06_$$.trace"
        exit 1
    fi
done
rm -f "${TMPDIR:-/tmp}/hw06_$$.trace"
