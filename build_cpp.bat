@echo off
REM Build C++ components
REM Скрипт збирання C++ компонентів
REM Скрипт сборки C++ компонентов

echo Building CTON-SDK C++ Core...
echo =============================

REM Create build directory
if not exist "cpp\build" mkdir "cpp\build"

REM Change to build directory
cd cpp\build

REM Configure with CMake
echo Configuring with CMake...
cmake .. -G "Visual Studio 17 2022" -A x64

REM Build the project
echo Building the project...
cmake --build . --config Release

REM Copy DLL to Java resources
echo Copying DLL to Java resources...
copy Release\cton-sdk-core.dll ..\..\java\src\main\resources\
copy Release\cton-sdk-core.dll ..\..\java\src\main\resources\win32-x86-64\

echo.
echo Build completed successfully!
echo.