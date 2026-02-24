@echo off

if not exist build mkdir build
cd build
cmake ..
cmake --build .

echo.
echo --- Running ---
Debug\imported_targets.exe 2>nul || imported_targets.exe
cd ..
