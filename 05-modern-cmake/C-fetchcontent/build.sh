#!/bin/bash
set -e
echo "Note: First build will download dependencies (may take a moment)"
echo ""
mkdir -p build && cd build
cmake ..
cmake --build .
echo ""
echo "--- Running ---"
./app
