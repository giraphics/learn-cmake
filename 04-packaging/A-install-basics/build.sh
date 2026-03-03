#!/bin/bash
set -e
mkdir -p build && cd build
cmake ..
cmake --build .

echo ""
echo "--- Creating packages ---"
cpack

echo ""
echo "--- Created packages ---"
ls -la *.tar.gz *.zip 2>/dev/null || echo "Check the build directory for packages"
