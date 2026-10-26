# CTON-SDK C++ Core Library
# Author: Андрій Будильников (Sparky)

cmake_minimum_required(VERSION 3.22)

project(cton-sdk-core)
set(CMAKE_CXX_STANDARD 17)
# OpenSSL is built no-shared, which uses the static CRT (/MT).
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Handle Cyrillic characters in path
cmake_policy(SET CMP0049 NEW)

# Listed explicitly: GLOB returns nothing when the project path contains
# characters outside the system ANSI code page.
set(SOURCES
    src/Address.cpp
    src/Boc.cpp
    src/Cell.cpp
    src/Crypto.cpp
    src/Mnemonic.cpp
    src/NativeInterface.cpp
    src/Slice.cpp
)
set(HEADERS
    include/Address.h
    include/Boc.h
    include/Cell.h
    include/Crypto.h
    include/Mnemonic.h
    include/NativeInterface.h
    include/Slice.h
)

# Create library
add_library(cton-sdk-core SHARED ${SOURCES} ${HEADERS})

# Define export macro for Windows DLL
target_compile_definitions(cton-sdk-core PRIVATE CTON_SDK_CORE_EXPORTS)

# Add debug information
target_compile_options(cton-sdk-core PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/Zi>)
target_link_options(cton-sdk-core PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/DEBUG>)

# Create test executable
add_executable(cton-sdk-test src/main.cpp)
target_link_libraries(cton-sdk-test cton-sdk-core)

# Create demo executable
add_executable(cton-sdk-demo src/demo.cpp)
target_link_libraries(cton-sdk-demo cton-sdk-core)

# Include directories
target_include_directories(cton-sdk-core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Find OpenSSL with more flexible options
set(OPENSSL_USE_STATIC_LIBS TRUE)

# OpenSSL 4.1.0 (23 Oct 2026). vcpkg port is still 3.6.x, so a local build wins.
# The source tree is openssl-4.1.0; nmake install lands in openssl-4.1.0-dist.
if(EXISTS "${CMAKE_SOURCE_DIR}/../openssl-4.1.0-dist")
    set(OPENSSL_ROOT_DIR "${CMAKE_SOURCE_DIR}/../openssl-4.1.0-dist")
    message(STATUS "Looking for OpenSSL 4.1.0 in: ${OPENSSL_ROOT_DIR}")
    find_package(OpenSSL 4.1 QUIET)
endif()

if(NOT OpenSSL_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/../openssl-4.1.0/lib")
    set(OPENSSL_ROOT_DIR "${CMAKE_SOURCE_DIR}/../openssl-4.1.0")
    message(STATUS "Looking for OpenSSL 4.1.0 in: ${OPENSSL_ROOT_DIR}")
    find_package(OpenSSL 4.1 QUIET)
endif()

if(NOT OpenSSL_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/../openssl")
    set(OPENSSL_ROOT_DIR "${CMAKE_SOURCE_DIR}/../openssl")
    message(STATUS "Looking for OpenSSL in project directory: ${OPENSSL_ROOT_DIR}")
    find_package(OpenSSL 4.1 QUIET)
endif()

# If still not found, try to find it in vcpkg installation
if(NOT OpenSSL_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/../vcpkg/installed/x64-windows")
    set(OPENSSL_ROOT_DIR "${CMAKE_SOURCE_DIR}/../vcpkg/installed/x64-windows")
    message(STATUS "Looking for OpenSSL in vcpkg directory: ${OPENSSL_ROOT_DIR}")
    find_package(OpenSSL 4.1 QUIET)
endif()

# If still not found, try system locations. 3.6 is not accepted.
if(NOT OpenSSL_FOUND)
    message(STATUS "Looking for OpenSSL 4.1 or newer in system locations")
    find_package(OpenSSL 4.1 QUIET)
endif()

# Link OpenSSL libraries
if(OPENSSL_FOUND)
    # For modern CMake versions, use imported targets if available
    if(TARGET OpenSSL::Crypto AND TARGET OpenSSL::SSL)
        target_link_libraries(cton-sdk-core PRIVATE OpenSSL::Crypto OpenSSL::SSL)
    else()
        # Fallback to direct linking
        target_link_libraries(cton-sdk-core PRIVATE ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})
        target_include_directories(cton-sdk-core PRIVATE ${OPENSSL_INCLUDE_DIR})
    endif()
    
    target_compile_definitions(cton-sdk-core PRIVATE OPENSSL_AVAILABLE=1)
    
    # Also set for test executables that use OpenSSL
    target_compile_definitions(cton-sdk-test PRIVATE OPENSSL_AVAILABLE=1)
    target_compile_definitions(cton-sdk-demo PRIVATE OPENSSL_AVAILABLE=1)
    
    message(STATUS "Using OpenSSL libraries: ${OpenSSL_LIBRARIES}")
    message(STATUS "Using OpenSSL version: ${OPENSSL_VERSION}")
    message(STATUS "Using OpenSSL include dirs: ${OpenSSL_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "OpenSSL 4.1 or newer was not found. vcpkg still packages 3.6.x. Build openssl-4.1.0 with Perl and NASM (perl Configure VC-WIN64A no-shared --prefix=<repo>/../openssl-4.1.0) and point OPENSSL_ROOT_DIR there.")
endif()

target_include_directories(cton-sdk-test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(cton-sdk-demo PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Compiler options
target_compile_options(cton-sdk-core PRIVATE 
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /wd4100 /wd4996 /wd4267>  # Disable various warnings
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)