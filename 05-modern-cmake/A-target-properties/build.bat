@echo off
if not exist build mkdir build
cd build
cmake ..
cmake --build .
echo.
echo --- Running ---
Debug\app.exe 2>nul || app.exe
cd ..
