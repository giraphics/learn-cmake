#!/bin/bash
set -e

# Check if Ninja is available
if command -v ninja &> /dev/null; then
    GENERATOR="Ninja"
    echo "Ninja found — using Ninja generator"
else
    GENERATOR="Unix Makefiles"
    echo "Ninja not found — falling back to Unix Makefiles"
fi

mkdir -p build && cd build
cmake .. -G "$GENERATOR"
cmake --build .
echo ""
echo "--- Running ---"
./build_ninja
