#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/../vect2"
echo "Compiling vect2..."
c++ -Wall -Wextra -Werror *.cpp -o vect2_test || { echo "compile failed"; exit 2; }
echo "Running vect2 main and checking output contains expected tokens"
OUT=$(./vect2_test)
echo "$OUT"
echo "$OUT" | grep -q "v1:" || { echo "expected output missing"; exit 3; }
echo "vect2 tests passed"
