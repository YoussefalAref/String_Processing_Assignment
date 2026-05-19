#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT_DIR"

echo "Building..."
make

echo
echo "Starting interactive app..."
echo "Tip: choose option 1 and load input.txt for a fast demo."
./string_matching
