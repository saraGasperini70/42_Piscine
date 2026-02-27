#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/../bsq"
echo "Building bsq with make..."
make || { echo "make failed"; exit 2; }
if [ -f bsq ]; then
  echo "Running bsq with map.txt"
  ./bsq map.txt || { echo "bsq run failed"; exit 3; }
  echo "bsq tests passed"
else
  echo "bsq executable missing"; exit 4
fi
