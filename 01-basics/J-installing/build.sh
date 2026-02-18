#!/bin/bash
set -e
INSTALL_DIR=$(pwd)/install

mkdir -p build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
cmake --build .
cmake --install .

echo ""
echo "--- Installed files ---"
find $INSTALL_DIR -type f | sort

echo ""
echo "--- Running installed binary ---"
$INSTALL_DIR/bin/hello_binary
