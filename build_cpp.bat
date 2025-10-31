@echo off
REM Build script for CTON-SDK C++ Core
REM Author: Андрій Будильников (Sparky)

echo Building CTON-SDK C++ Core...

REM Check if vcpkg directory exists
if not exist vcpkg (
    echo Cloning vcpkg...
    git clone https://github.com/Microsoft/vcpkg.git
    if %ERRORLEVEL% NEQ 0 (
        echo Failed to clone vcpkg
        exit /b %ERRORLEVEL%
    )
)

REM Check if vcpkg is bootstrapped
cd vcpkg
if not exist vcpkg.exe (
    echo Bootstrapping vcpkg...
    cmd /c bootstrap-vcpkg.bat
    if %ERRORLEVEL% NEQ 0 (
        echo Failed to bootstrap vcpkg
        cd ..
        exit /b %ERRORLEVEL%
    )
)

REM Check if OpenSSL is installed
if not exist installed\x64-windows\lib\ (
    echo Installing OpenSSL via vcpkg...
    cmd /c vcpkg install openssl:x64-windows
    if %ERRORLEVEL% NEQ 0 (
        echo Failed to install OpenSSL
        cd ..
        exit /b %ERRORLEVEL%
    )
    echo OpenSSL installed successfully!
) else (
    echo OpenSSL already installed
)

cd ..

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