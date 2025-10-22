# CTON-SDK C++ Core Library
# Author: Андрій Будильников (Sparky)
# cmake_minimum_required(VERSION 3.10)

project(cton-sdk-core)
set(CMAKE_CXX_STANDARD 17)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

# Create library
add_library(cton-sdk-core SHARED ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(cton-sdk-core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Compiler options
target_compile_options(cton-sdk-core PRIVATE 
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

# Link libraries (example - to be expanded)
# target_link_libraries(cton-sdk-core PRIVATE OpenSSL::Crypto)