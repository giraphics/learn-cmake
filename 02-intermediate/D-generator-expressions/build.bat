@echo off

if not exist build mkdir build
cd build
cmake ..
cmake --build .

echo.
echo --- Running ---
Debug\genexpr.exe 2>nul || genexpr.exe
cd ..
