#!/bin/sh
set -eu
program=${1:-./balance}
case "$program" in
  /*) ;;
  *) program="$(pwd)/${program#./}" ;;
esac
passed=0
failed=0
for input in inputs/*.txt; do
    name=${input##*/}
    name=${name%.txt}
    tmp=$(mktemp)
    status=0
    "$program" < "$input" > "$tmp" || status=$?
    expected_status=$(cat "expected/$name.status")
    if diff -u "expected/$name.out" "$tmp" && [ "$status" -eq "$expected_status" ]; then
        printf 'PASS: %s (exit %s)\n' "$name" "$status"
        passed=$((passed + 1))
    else
        printf 'FAIL: %s (expected exit %s, got %s)\n' "$name" "$expected_status" "$status"
        failed=$((failed + 1))
    fi
    rm -f "$tmp"
done
printf 'Regression summary: %s passed, %s failed\n' "$passed" "$failed"
[ "$failed" -eq 0 ]
