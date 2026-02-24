@echo off

if not exist build mkdir build
cd build
cmake ..
cmake --build .

echo.
echo --- Running ---
Debug\subprojects.exe 2>nul || subprojects.exe
cd ..
