#!/bin/bash
set -e

BUILD_TYPE=${1:-Debug}
echo "Building with type: $BUILD_TYPE"

mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE
cmake --build .
echo ""
echo "--- Running ---"
./build_type
