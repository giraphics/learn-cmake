@echo off

if not exist build mkdir build
cd build
cmake ..
cmake --build .

echo.
echo --- Running ---
Debug\code_generation.exe 2>nul || code_generation.exe
cd ..
