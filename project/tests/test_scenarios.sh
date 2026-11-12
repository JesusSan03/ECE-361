#!/bin/sh
# Compare the complete output, not just one example line.
set -eu
for name in normal rain threshold midnight wrap unexpected; do
    ./irrigation "scenarios/$name.txt" > "scenarios/$name.actual"
    if ! diff -u "scenarios/$name.expected" "scenarios/$name.actual"; then
        rm -f "scenarios/$name.actual"
        echo "FAILED: $name" >&2
        exit 1
    fi
    rm -f "scenarios/$name.actual"
    echo "PASS scenario: $name"
done
# --report is intentionally unavailable until Week 9.
if ./irrigation scenarios/normal.txt --report > /dev/null 2>/dev/null; then
    echo "FAILED: week 9 report should not claim to work" >&2; exit 1
fi
echo "PASS expected unfinished report rejection"
echo "Scenario checks passed: 7, failed: 0"
