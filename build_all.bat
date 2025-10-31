@echo off
REM Build script for CTON-SDK
REM Author: Андрій Будильников (Sparky)

echo Building CTON-SDK...
echo ===================

REM Build C++ core
echo.
echo [1/3] Building C++ Core...
call build_cpp.bat

if %ERRORLEVEL% NEQ 0 (
    echo C++ build failed!
    exit /b %ERRORLEVEL%
)

REM Copy DLL to Java resources
echo.
echo [2/3] Copying native libraries...
copy cpp\build\Release\cton-sdk-core.dll java\src\main\resources\ >nul
copy cpp\build\Release\cton-sdk-core.dll java\src\main\resources\win32-x86-64\ >nul

if %ERRORLEVEL% EQU 0 (
    echo Native libraries copied successfully!
) else (
    echo Failed to copy native libraries!
    exit /b %ERRORLEVEL%
)

REM Build Java components
echo.
echo [3/3] Building Java Components...
call build_java.bat

if %ERRORLEVEL% NEQ 0 (
    echo Java build failed!
    exit /b %ERRORLEVEL%
)

echo.
echo ===================
echo Build completed successfully!