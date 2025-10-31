@echo off
REM Install OpenSSL via vcpkg
REM Author: Андрій Будильников (Sparky)

echo Installing OpenSSL via vcpkg...

cd vcpkg

REM Bootstrap vcpkg if not already done
if not exist vcpkg.exe (
    echo Bootstrapping vcpkg...
    call bootstrap-vcpkg.bat
)

REM Install OpenSSL
echo Installing OpenSSL...
vcpkg install openssl:x64-windows

if %ERRORLEVEL% EQU 0 (
    echo OpenSSL installed successfully!
    echo Libraries should be available at vcpkg\installed\x64-windows\lib\
) else (
    echo Failed to install OpenSSL
    exit /b %ERRORLEVEL%
)

cd ..