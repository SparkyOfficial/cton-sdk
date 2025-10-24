@echo off
REM Build and run comprehensive test for CTON-SDK
REM Author: Андрій Будильников (Sparky)

echo Building CTON-SDK comprehensive test...
echo ======================================

REM Create build directory if it doesn't exist
if not exist "build" mkdir "build"
cd build

REM Run CMake to generate build files
cmake .. -G "Visual Studio 17 2022" -A x64

REM Build the test
cmake --build . --config Release

REM Check if build was successful
if %errorlevel% neq 0 (
    echo Error: Build failed!
    cd ..
    exit /b %errorlevel%
)

echo.
echo Running comprehensive test...
echo ===========================

REM Run the test
Release\comprehensive_test.exe

REM Check if test was successful
if %errorlevel% neq 0 (
    echo Error: Test failed!
    cd ..
    exit /b %errorlevel%
)

echo.
echo Test completed successfully!

cd ..