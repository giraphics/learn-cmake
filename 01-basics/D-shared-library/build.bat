@echo off
if not exist build mkdir build
cd build
cmake ..
cmake --build .
echo.
echo --- Running ---
Debug\hello_binary.exe 2>nul || hello_binary.exe
cd ..
