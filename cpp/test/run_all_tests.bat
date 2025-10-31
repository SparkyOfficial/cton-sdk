@echo off
REM Run all C++ tests for CTON-SDK
REM Author: Андрій Будильников (Sparky)

echo Running CTON-SDK C++ Tests...
echo =============================

set FAILED=0

REM Change to build directory
cd ..\build

REM Run unit tests
echo.
echo Running Cell Tests...
echo ------------------
cell_test.exe
if %ERRORLEVEL% NEQ 0 set FAILED=1

echo.
echo Running Address Tests...
echo ---------------------
address_test.exe
if %ERRORLEVEL% NEQ 0 set FAILED=1

echo.
echo Running Crypto Tests...
echo --------------------
crypto_test.exe
if %ERRORLEVEL% NEQ 0 set FAILED=1

echo.
echo Running BOC Tests...
echo -----------------
boc_test.exe
if %ERRORLEVEL% NEQ 0 set FAILED=1

echo.
echo Running Crypto Security Tests...
echo -----------------------------
crypto_security_test.exe
if %ERRORLEVEL% NEQ 0 set FAILED=1

echo.
echo Running Ed25519 Tests...
echo ----------------------
ed25519_test.exe
if %ERRORLEVEL% NEQ 0 set FAILED=1

echo.
echo Running BOC Compliance Tests...
echo -----------------------------
boc_compliance_test.exe
if %ERRORLEVEL% NEQ 0 set FAILED=1

echo.
echo Running Mnemonic Tests...
echo -----------------------
mnemonic_test.exe
if %ERRORLEVEL% NEQ 0 set FAILED=1

echo.
echo Running BIP-39 Compliance Tests...
echo --------------------------------
bip39_compliance_test.exe
if %ERRORLEVEL% NEQ 0 set FAILED=1

echo.
echo Running Crypto Algorithms Tests...
echo --------------------------------
crypto_algorithms_test.exe
if %ERRORLEVEL% NEQ 0 set FAILED=1

if %FAILED% EQU 0 (
    echo.
    echo ALL TESTS PASSED! 🎉
    echo ===================
) else (
    echo.
    echo SOME TESTS FAILED! ❌
    exit /b 1
)