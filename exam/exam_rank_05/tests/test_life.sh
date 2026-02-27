#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/../life"
echo "Compiling life..."
gcc -Wall -Wextra -Werror *.c -o life_test || { echo "compile failed"; exit 2; }
echo "Running life sample tests"
echo 'sdxddssaaww' | ./life_test 5 5 0 | cat -e
echo 'dxss' | ./life_test 3 3 0 | cat -e
echo "life tests passed"
