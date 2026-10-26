@echo off
REM OpenSSL 4.1.0 (released 23 Oct 2026) is not in vcpkg yet.
REM The vcpkg port is still 3.6.5, and CMake rejects anything older than 4.1.
REM Author: Андрій Будильников (Sparky)

echo OpenSSL 4.1.0 is not packaged by vcpkg yet.
echo Build it from the Visual Studio x64 developer prompt.
echo Perl and NASM must be on PATH.
echo.
echo   git clone --depth 1 --branch openssl-4.1.0 https://github.com/openssl/openssl.git openssl-4.1.0-src
echo   cd openssl-4.1.0-src
echo   perl Configure VC-WIN64A no-shared --prefix=..\openssl-4.1.0 --openssldir=..\openssl-4.1.0\ssl
echo   nmake
echo   nmake install
echo.
echo CMake then picks up openssl-4.1.0 next to the cpp directory.
exit /b 0
