@echo off

if not exist build mkdir build
cd build
cmake ..
cmake --build .
echo.
echo --- Running ---
Debug\compile_flags.exe 2>nul || compile_flags.exe
cd ..
