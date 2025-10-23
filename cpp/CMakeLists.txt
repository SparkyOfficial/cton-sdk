# CTON-SDK C++ Core Library
# Author: Андрій Будильников (Sparky)
# cmake_minimum_required(VERSION 3.10)

project(cton-sdk-core)
set(CMAKE_CXX_STANDARD 17)

# Find OpenSSL package
find_package(OpenSSL REQUIRED)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

# Exclude main.cpp from library sources
list(FILTER SOURCES EXCLUDE REGEX "main.cpp")

# Create library
add_library(cton-sdk-core SHARED ${SOURCES} ${HEADERS})

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
    ${OPENSSL_INCLUDE_DIR}
)

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
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

# Link libraries
target_link_libraries(cton-sdk-core PRIVATE 
    OpenSSL::Crypto
)

# Define USE_OPENSSL for conditional compilation
target_compile_definitions(cton-sdk-core PRIVATE USE_OPENSSL)