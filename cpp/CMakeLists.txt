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

# Disable OpenSSL for now since we don't have a complete installation
# In the future, you can uncomment the OpenSSL section below and properly install OpenSSL
# 
# # Try to find OpenSSL in common locations
# set(OPENSSL_ROOT_DIR ${OPENSSL_ROOT_DIR} "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0")
# set(OPENSSL_INCLUDE_DIR ${OPENSSL_INCLUDE_DIR} "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include")
# 
# # Try to find OpenSSL
# find_package(OpenSSL QUIET)
# 
# # Conditionally include OpenSSL if found
# if(OPENSSL_FOUND)
#     target_include_directories(cton-sdk-core PUBLIC ${OPENSSL_INCLUDE_DIR})
#     target_link_libraries(cton-sdk-core PRIVATE OpenSSL::Crypto)
#     target_compile_definitions(cton-sdk-core PRIVATE USE_OPENSSL)
#     message(STATUS "OpenSSL found: ${OPENSSL_VERSION}")
# else()
#     # Try manual OpenSSL configuration
#     if(EXISTS "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include/openssl")
#         target_include_directories(cton-sdk-core PUBLIC "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include")
#         # Check if we have precompiled libraries
#         if(EXISTS "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/libcrypto.lib")
#             target_link_libraries(cton-sdk-core PRIVATE "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/libcrypto.lib")
#         elseif(EXISTS "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/lib/libcrypto.lib")
#             target_link_libraries(cton-sdk-core PRIVATE "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/lib/libcrypto.lib")
#         else()
#             message(WARNING "OpenSSL libraries not found, crypto functionality will be limited")
#         endif()
#         target_compile_definitions(cton-sdk-core PRIVATE USE_OPENSSL)
#         message(STATUS "Using manual OpenSSL configuration")
#     else()
#         message(WARNING "OpenSSL not found, crypto functionality will be limited")
#     endif()
# endif()

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