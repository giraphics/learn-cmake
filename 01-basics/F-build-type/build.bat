@echo off
set BUILD_TYPE=%1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Debug
echo Building with type: %BUILD_TYPE%

if not exist build mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
cmake --build . --config %BUILD_TYPE%
echo.
echo --- Running ---
%BUILD_TYPE%\build_type.exe 2>nul || build_type.exe
cd ..
