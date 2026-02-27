#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/../polyset"
echo "Compiling polyset..."
c++ -Wall -Wextra -Werror *.cpp -o polyset_test || { echo "compile failed"; exit 2; }
echo "Running polyset basic runs"
./polyset_test 5 3 64 2534 436 || { echo "run failed"; exit 3; }
./polyset_test 1 || true
echo "polyset tests passed"
