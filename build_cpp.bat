@echo off
REM Build script for CTON-SDK C++ Core
REM Author: Андрій Будильников (Sparky)

echo Building CTON-SDK C++ Core...

REM Create build directory
if not exist cpp\build mkdir cpp\build

REM Build C++ core
cd cpp\build
cmake ..
cmake --build . --config Release

REM Check if build was successful
if %ERRORLEVEL% EQU 0 (
    echo C++ core built successfully!
) else (
    echo C++ core build failed!
    exit /b %ERRORLEVEL%
)

cd ..\..

echo C++ build completed successfully!