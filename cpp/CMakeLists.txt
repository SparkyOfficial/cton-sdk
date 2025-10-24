# CTON-SDK C++ Core Library
# Author: Андрій Будильников (Sparky)

cmake_minimum_required(VERSION 3.10)

project(cton-sdk-core)
set(CMAKE_CXX_STANDARD 17)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

# Exclude main.cpp from library sources
list(FILTER SOURCES EXCLUDE REGEX "main.cpp")

# Create library
add_library(cton-sdk-core SHARED ${SOURCES} ${HEADERS})

# Define export macro for Windows DLL
target_compile_definitions(cton-sdk-core PRIVATE CTON_SDK_CORE_EXPORTS)

# Create test executable
add_executable(cton-sdk-test src/main.cpp)
target_link_libraries(cton-sdk-test cton-sdk-core)

# Create unit test executables
add_executable(cell_test test/CellTest.cpp)
target_link_libraries(cell_test cton-sdk-core)

add_executable(address_test test/AddressTest.cpp)
target_link_libraries(address_test cton-sdk-core)

add_executable(crypto_test test/CryptoTest.cpp)
target_link_libraries(crypto_test cton-sdk-core)

add_executable(boc_test test/BocTest.cpp)
target_link_libraries(boc_test cton-sdk-core)

add_executable(native_interface_test test/NativeInterfaceTest.cpp)
target_link_libraries(native_interface_test cton-sdk-core)

# Include directories
target_include_directories(cton-sdk-core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Manual OpenSSL configuration
set(OPENSSL_ROOT_DIR "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0")
set(OPENSSL_INCLUDE_DIR "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include")

# Include OpenSSL headers
target_include_directories(cton-sdk-core PUBLIC ${OPENSSL_INCLUDE_DIR})

# Try to find precompiled OpenSSL libraries in common locations
set(OPENSSL_LIB_PATHS
    "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/lib"
    "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/lib64"
    "C:/Users/Богдан/Desktop/cton-sdk/vcpkg/installed/x64-windows/lib"
    "C:/Users/Богдан/Desktop/cton-sdk/vcpkg/installed/x64-windows/lib/manual-link"
    "C:/OpenSSL/lib"
)

# Try to find libraries in each path
foreach(LIB_PATH ${OPENSSL_LIB_PATHS})
    if(EXISTS "${LIB_PATH}/libcrypto.lib")
        target_link_libraries(cton-sdk-core PRIVATE 
            "${LIB_PATH}/libcrypto.lib"
            "${LIB_PATH}/libssl.lib")
        target_compile_definitions(cton-sdk-core PRIVATE USE_OPENSSL)
        message(STATUS "Using OpenSSL libraries from: ${LIB_PATH}")
        break()
    elseif(EXISTS "${LIB_PATH}/libcrypto.a")
        target_link_libraries(cton-sdk-core PRIVATE 
            "${LIB_PATH}/libcrypto.a"
            "${LIB_PATH}/libssl.a")
        target_compile_definitions(cton-sdk-core PRIVATE USE_OPENSSL)
        message(STATUS "Using OpenSSL libraries from: ${LIB_PATH}")
        break()
    endif()
endforeach()

# If no precompiled libraries found, try to find them with find_package
if(NOT TARGET OpenSSL::Crypto)
    find_package(OpenSSL QUIET)
    if(OPENSSL_FOUND)
        target_link_libraries(cton-sdk-core PRIVATE OpenSSL::Crypto OpenSSL::SSL)
        target_compile_definitions(cton-sdk-core PRIVATE USE_OPENSSL)
        message(STATUS "OpenSSL found: ${OPENSSL_VERSION}")
    else()
        message(WARNING "OpenSSL libraries not found, crypto functionality will be limited")
    endif()
endif()

# If we still don't have OpenSSL, try to build it or use a fallback
if(NOT TARGET OpenSSL::Crypto)
    message(STATUS "Using basic random number generation as fallback for crypto")
endif()

target_include_directories(cell_test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(address_test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(crypto_test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(boc_test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(native_interface_test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Compiler options
target_compile_options(cton-sdk-core PRIVATE 
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /wd4100 /wd4996 /wd4267>  # Disable various warnings
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)