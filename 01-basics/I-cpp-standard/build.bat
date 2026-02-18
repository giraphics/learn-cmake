@echo off

if not exist build mkdir build
cd build
cmake ..
cmake --build .
echo.
echo --- Running ---
Debug\cpp_standard.exe 2>nul || cpp_standard.exe
cd ..
