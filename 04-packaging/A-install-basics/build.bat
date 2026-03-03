@echo off
if not exist build mkdir build
cd build
cmake ..
cmake --build .

echo.
echo --- Creating packages ---
cpack -C Debug

echo.
echo --- Created packages ---
dir /b *.zip *.tar.gz 2>nul
cd ..
