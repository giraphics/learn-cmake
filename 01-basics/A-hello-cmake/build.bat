@echo off
if not exist build mkdir build
cd build
cmake ..
cmake --build .
echo.
echo --- Running ---
Debug\hello_cmake.exe 2>nul || hello_cmake.exe
cd ..
