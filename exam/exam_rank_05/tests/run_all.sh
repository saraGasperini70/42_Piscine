#!/bin/bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SCRIPTS=(
  test_polyset.sh
  test_bigint.sh
#  test_vect2.sh
  test_bsq.sh
  test_life.sh
)
cd "$ROOT"
echo "Running all tests in $ROOT/tests"
for s in "${SCRIPTS[@]}"; do
  echo "--- $s ---"
  bash "tests/$s"
done
echo "All tests completed successfully."
