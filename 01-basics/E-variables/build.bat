@echo off
if not exist build mkdir build
cd build
cmake ..
cmake --build .
echo.
echo --- Running ---
Debug\hello_variables.exe 2>nul || hello_variables.exe
cd ..
