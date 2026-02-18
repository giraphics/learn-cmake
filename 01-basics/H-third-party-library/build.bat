@echo off

if not exist build mkdir build
cd build
cmake ..
cmake --build .
echo.
echo --- Running ---
Debug\third_party.exe 2>nul || third_party.exe
cd ..
