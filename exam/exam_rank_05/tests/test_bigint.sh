#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/../bigint"
echo "Compiling bigint..."
c++ -Wall -Wextra -Werror *.cpp -o bigint_test || { echo "compile failed"; exit 2; }
echo "Running bigint main"
./bigint_test || { echo "run failed"; exit 3; }
echo "bigint tests passed"
