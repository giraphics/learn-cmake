@echo off
if not exist build mkdir build
cd build
cmake ..
cmake --build .
echo.
echo --- Running CTest ---
ctest --output-on-failure -C Debug
echo.
echo --- Verbose output ---
ctest -V -C Debug
cd ..
