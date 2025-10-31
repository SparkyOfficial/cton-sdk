@echo off
REM Build script for CTON-SDK C++ Core
REM Author: Андрій Будильников (Sparky)

echo Building CTON-SDK C++ Core...

REM Check if vcpkg installed OpenSSL
if not exist vcpkg\installed\x64-windows\lib\ (
    echo OpenSSL not found in vcpkg. Installing...
    call install_openssl.bat
    if %ERRORLEVEL% NEQ 0 (
        echo Failed to install OpenSSL
        exit /b %ERRORLEVEL%
    )
)

REM Create build directory
if not exist cpp\build mkdir cpp\build

REM Build C++ core
cd cpp\build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../../vcpkg/scripts/buildsystems/vcpkg.cmake
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