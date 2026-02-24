@echo off
where ninja >nul 2>nul
if %errorlevel% equ 0 (
    set GENERATOR=Ninja
    echo Ninja found — using Ninja generator
) else (
    set GENERATOR=NMake Makefiles
    echo Ninja not found — using default generator
)

if not exist build mkdir build
cd build
cmake .. -G "%GENERATOR%"
cmake --build .
echo.
echo --- Running ---
build_ninja.exe
cd ..
