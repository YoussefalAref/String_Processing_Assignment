#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT_DIR"

echo "[1/3] Building project..."
make

echo
echo "[2/3] Test: Boyer-Moore with overlapping pattern AAA"
bm_output="$(
  {
    echo "1"
    echo "input.txt"
    echo "3"
    echo "AAA"
    echo "6"
  } | ./string_matching
)"

echo "$bm_output" | grep -E "Algorithm: Boyer-Moore|Number of Matches:"

echo
echo "[3/3] Test: Rabin-Karp with pattern DATA"
rk_output="$(
  {
    echo "1"
    echo "input.txt"
    echo "4"
    echo "DATA"
    echo "6"
  } | ./string_matching
)"

echo "$rk_output" | grep -E "Algorithm: Rabin-Karp|Number of Matches:"

echo
echo "All scripted tests completed."
