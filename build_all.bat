@echo off
REM Build script for entire CTON-SDK project
REM Author: Андрій Будильников (Sparky)

echo Building CTON-SDK Project...
echo ===========================

set FAILED=0

echo.
echo Building C++ Core...
echo ------------------
call build_cpp.bat
if %ERRORLEVEL% NEQ 0 set FAILED=1

if %FAILED% EQU 1 (
    echo.
    echo C++ Core build failed! Skipping Java builds.
    exit /b 1
)

echo.
echo Building Core SDK...
echo ------------------
cd java
mvn clean install
if %ERRORLEVEL% NEQ 0 set FAILED=1
cd ..

if %FAILED% EQU 1 (
    echo.
    echo Core SDK build failed! Skipping API client build.
    exit /b 1
)

echo.
echo Building API Client...
echo --------------------
cd api-client
mvn clean install
if %ERRORLEVEL% NEQ 0 set FAILED=1
cd ..

if %FAILED% EQU 0 (
    echo.
    echo ALL COMPONENTS BUILT SUCCESSFULLY!
    echo =================================
    echo C++ Core: Built
    echo Core SDK: Built
    echo API Client: Built
) else (
    echo.
    echo SOME COMPONENTS FAILED TO BUILD!
    exit /b 1
)