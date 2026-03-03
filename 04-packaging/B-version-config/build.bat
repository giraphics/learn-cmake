@echo off
set INSTALL_DIR=%cd%\install

if not exist build mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR%
cmake --build .
cmake --install .

echo.
echo --- Installed file tree ---
dir /s /b "%INSTALL_DIR%"

echo.
echo --- Running ---
"%INSTALL_DIR%\bin\myapp.exe"
cd ..
