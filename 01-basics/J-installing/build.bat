@echo off
set INSTALL_DIR=%cd%\install

if not exist build mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR%
cmake --build .
cmake --install .

echo.
echo --- Installed files ---
dir /s /b "%INSTALL_DIR%"

echo.
echo --- Running installed binary ---
"%INSTALL_DIR%\bin\hello_binary.exe"
cd ..
