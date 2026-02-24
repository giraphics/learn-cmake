#!/bin/bash
set -e

mkdir -p build && cd build
cmake ..
cmake --build .

echo ""
echo "--- Running ---"
./code_generation
