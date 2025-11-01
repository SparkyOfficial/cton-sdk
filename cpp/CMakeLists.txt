# CTON-SDK C++ Core Library
# Author: Андрій Будильников (Sparky)

cmake_minimum_required(VERSION 3.10)

project(cton-sdk-core)
set(CMAKE_CXX_STANDARD 17)

# Handle Cyrillic characters in path
cmake_policy(SET CMP0049 NEW)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

# Exclude main.cpp and test files from library sources
list(FILTER SOURCES EXCLUDE REGEX "main.cpp")
list(FILTER SOURCES EXCLUDE REGEX "test")
list(FILTER SOURCES EXCLUDE REGEX "demo.cpp")

# Create library
add_library(cton-sdk-core SHARED ${SOURCES} ${HEADERS})

# Define export macro for Windows DLL
target_compile_definitions(cton-sdk-core PRIVATE CTON_SDK_CORE_EXPORTS)

# Create test executable
add_executable(cton-sdk-test src/main.cpp)
target_link_libraries(cton-sdk-test cton-sdk-core)

# Create demo executable
add_executable(cton-sdk-demo src/demo.cpp)
target_link_libraries(cton-sdk-demo cton-sdk-core)

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

# Create comprehensive test executable
add_executable(comprehensive_test test/ComprehensiveTest.cpp)
target_link_libraries(comprehensive_test cton-sdk-core)

# Create Ed25519 test executable
add_executable(ed25519_test test/Ed25519Test.cpp)
target_link_libraries(ed25519_test cton-sdk-core)

# Create BOC compliance test executable
add_executable(boc_compliance_test test/BocComplianceTest.cpp)
target_link_libraries(boc_compliance_test cton-sdk-core)

# Create mnemonic test executable
add_executable(mnemonic_test test/MnemonicTest.cpp)
target_link_libraries(mnemonic_test cton-sdk-core)

# Create BIP-39 compliance test executable
add_executable(bip39_compliance_test test/Bip39ComplianceTest.cpp)
target_link_libraries(bip39_compliance_test cton-sdk-core)

# Create additional cryptographic algorithms test executable
add_executable(crypto_algorithms_test test/CryptoAlgorithmsTest.cpp)
target_link_libraries(crypto_algorithms_test cton-sdk-core)

# Create BOC performance test executable
add_executable(boc_performance_test test/BocPerformanceTest.cpp)
target_link_libraries(boc_performance_test cton-sdk-core)

# Create benchmark test executable
add_executable(benchmark_test test/BenchmarkTest.cpp)
target_link_libraries(benchmark_test cton-sdk-core)

# Create integration test executable
add_executable(integration_test test/IntegrationTest.cpp)
target_link_libraries(integration_test cton-sdk-core)

# Create debug test executable
add_executable(debug_test debug_test.cpp)
target_link_libraries(debug_test cton-sdk-core)

# Create ChaCha20 test executable
add_executable(chacha20_test chacha20_test.cpp)
target_link_libraries(chacha20_test cton-sdk-core)

# Create JNA debug test executable
add_executable(jna_debug_test jna_debug_test.cpp)
target_link_libraries(jna_debug_test cton-sdk-core)

# Create ChaCha20 JNA test executable
add_executable(chacha20_jna_test chacha20_jna_test.cpp)
target_link_libraries(chacha20_jna_test cton-sdk-core)

# Create ChaCha20 debug test executable
add_executable(chacha20_debug_test chacha20_debug_test.cpp)
target_link_libraries(chacha20_debug_test cton-sdk-core)

# Create ChaCha20 simple test executable
add_executable(chacha20_simple_test chacha20_simple_test.cpp)
target_link_libraries(chacha20_simple_test cton-sdk-core)

# Create ChaCha20 double encryption test executable
add_executable(chacha20_double_encrypt_test chacha20_double_encrypt_test.cpp)
target_link_libraries(chacha20_double_encrypt_test cton-sdk-core)

# Create ChaCha20 Java mimic test executable
add_executable(chacha20_java_mimic_test chacha20_java_mimic_test.cpp)
target_link_libraries(chacha20_java_mimic_test cton-sdk-core)

# Create ChaCha20 Java mimic exact test executable
add_executable(chacha20_java_mimic_exact_test chacha20_java_mimic_exact_test.cpp)
target_link_libraries(chacha20_java_mimic_exact_test cton-sdk-core)

# Create crypto security test executable
add_executable(crypto_security_test test/CryptoSecurityTest.cpp)
target_link_libraries(crypto_security_test cton-sdk-core)

# Create SHA256 test executable
# add_executable(test_openssl test_openssl.cpp)
# target_link_libraries(test_openssl cton-sdk-core)
# target_include_directories(test_openssl PRIVATE ${OPENSSL_INCLUDE_DIR})

# Include directories
target_include_directories(cton-sdk-core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Find OpenSSL
find_package(OpenSSL QUIET)

# If OpenSSL is not found, try to find it in the project directory or vcpkg
if(NOT OpenSSL_FOUND)
    # Set default OpenSSL root directory
    if(NOT DEFINED OPENSSL_ROOT_DIR)
        # Check if OpenSSL is in the project directory
        if(EXISTS "${CMAKE_SOURCE_DIR}/../openssl-3.6.0")
            set(OPENSSL_ROOT_DIR "${CMAKE_SOURCE_DIR}/../openssl-3.6.0")
        # Check if OpenSSL is installed via vcpkg (Windows)
        elseif(EXISTS "${CMAKE_SOURCE_DIR}/../vcpkg/installed/x64-windows")
            set(OPENSSL_ROOT_DIR "${CMAKE_SOURCE_DIR}/../vcpkg/installed/x64-windows")
        # Check if OpenSSL is installed via vcpkg (Linux)
        elseif(EXISTS "${CMAKE_SOURCE_DIR}/../vcpkg/installed/x64-linux")
            set(OPENSSL_ROOT_DIR "${CMAKE_SOURCE_DIR}/../vcpkg/installed/x64-linux")
        endif()
    endif()
    
    # Set OpenSSL include directory
    if(EXISTS "${OPENSSL_ROOT_DIR}/include")
        set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
    endif()
    
    message(STATUS "OpenSSL root directory: ${OPENSSL_ROOT_DIR}")
    message(STATUS "OpenSSL include directory: ${OPENSSL_INCLUDE_DIR}")
endif()

# Include OpenSSL headers for all targets that need it
if(OPENSSL_INCLUDE_DIR)
    target_include_directories(cton-sdk-core PRIVATE ${OPENSSL_INCLUDE_DIR})
    target_include_directories(crypto_test PRIVATE ${OPENSSL_INCLUDE_DIR})
    target_include_directories(ed25519_test PRIVATE ${OPENSSL_INCLUDE_DIR})
    target_include_directories(crypto_algorithms_test PRIVATE ${OPENSSL_INCLUDE_DIR})
    target_include_directories(crypto_security_test PRIVATE ${OPENSSL_INCLUDE_DIR})
endif()

# If we couldn't find the libraries above, try to find them directly
if(NOT TARGET OpenSSL::Crypto)
    # Look for the DLL files directly in the OpenSSL directory
    file(GLOB OPENSSL_CRYPTO_LIBS "${OPENSSL_ROOT_DIR}/libcrypto*.lib")
    file(GLOB OPENSSL_SSL_LIBS "${OPENSSL_ROOT_DIR}/libssl*.lib")
    
    # Also look for .a files (Linux/macOS/MinGW)
    file(GLOB OPENSSL_CRYPTO_LIBS_A "${OPENSSL_ROOT_DIR}/libcrypto*.a")
    file(GLOB OPENSSL_SSL_LIBS_A "${OPENSSL_ROOT_DIR}/libssl*.a")
    
    # Look for import libraries (.dll.a) on Windows/MinGW
    file(GLOB OPENSSL_CRYPTO_DLL_A "${OPENSSL_ROOT_DIR}/libcrypto*.dll.a")
    file(GLOB OPENSSL_SSL_DLL_A "${OPENSSL_ROOT_DIR}/libssl*.dll.a")
    
    if(OPENSSL_CRYPTO_LIBS AND OPENSSL_SSL_LIBS)
        # Find specific libraries
        foreach(lib ${OPENSSL_CRYPTO_LIBS})
            if(lib MATCHES ".*libcrypto.*")
                set(OPENSSL_CRYPTO_LIB ${lib})
            elseif(lib MATCHES ".*libssl.*")
                set(OPENSSL_SSL_LIB ${lib})
            endif()
        endforeach()
        
        if(OPENSSL_CRYPTO_LIB AND OPENSSL_SSL_LIB)
            target_link_libraries(cton-sdk-core PRIVATE 
                ${OPENSSL_CRYPTO_LIB}
                ${OPENSSL_SSL_LIB})
            target_compile_definitions(cton-sdk-core PRIVATE OPENSSL_AVAILABLE=1)
            message(STATUS "Using OpenSSL libraries: ${OPENSSL_CRYPTO_LIB}, ${OPENSSL_SSL_LIB}")
            
            # Also set for test executables
            target_compile_definitions(crypto_test PRIVATE OPENSSL_AVAILABLE=1)
            target_compile_definitions(ed25519_test PRIVATE OPENSSL_AVAILABLE=1)
            target_compile_definitions(crypto_algorithms_test PRIVATE OPENSSL_AVAILABLE=1)
            target_compile_definitions(crypto_security_test PRIVATE OPENSSL_AVAILABLE=1)
        else()
            message(WARNING "Could not find required OpenSSL libraries in ${OPENSSL_ROOT_DIR}")
        endif()
    elseif(OPENSSL_CRYPTO_LIBS_A AND OPENSSL_SSL_LIBS_A)
        # Find specific libraries
        foreach(lib ${OPENSSL_CRYPTO_LIBS_A})
            if(lib MATCHES ".*libcrypto\\.a$")
                set(OPENSSL_CRYPTO_LIB ${lib})
            endif()
        endforeach()
        
        foreach(lib ${OPENSSL_SSL_LIBS_A})
            if(lib MATCHES ".*libssl\\.a$")
                set(OPENSSL_SSL_LIB ${lib})
            endif()
        endforeach()
        
        if(OPENSSL_CRYPTO_LIB AND OPENSSL_SSL_LIB)
            target_link_libraries(cton-sdk-core PRIVATE 
                ${OPENSSL_CRYPTO_LIB}
                ${OPENSSL_SSL_LIB})
            target_compile_definitions(cton-sdk-core PRIVATE OPENSSL_AVAILABLE=1)
            message(STATUS "Using OpenSSL libraries: ${OPENSSL_CRYPTO_LIB}, ${OPENSSL_SSL_LIB}")
            
            # Also set for test executables
            target_compile_definitions(crypto_test PRIVATE OPENSSL_AVAILABLE=1)
            target_compile_definitions(ed25519_test PRIVATE OPENSSL_AVAILABLE=1)
            target_compile_definitions(crypto_algorithms_test PRIVATE OPENSSL_AVAILABLE=1)
            target_compile_definitions(crypto_security_test PRIVATE OPENSSL_AVAILABLE=1)
        else()
            message(WARNING "Could not find required OpenSSL libraries in ${OPENSSL_ROOT_DIR}")
        endif()
    elseif(OPENSSL_CRYPTO_DLL_A AND OPENSSL_SSL_DLL_A)
        # Find specific import libraries for Windows/MinGW
        foreach(lib ${OPENSSL_CRYPTO_DLL_A})
            if(lib MATCHES ".*libcrypto.*\\.dll\\.a$")
                set(OPENSSL_CRYPTO_LIB ${lib})
            endif()
        endforeach()
        
        foreach(lib ${OPENSSL_SSL_DLL_A})
            if(lib MATCHES ".*libssl.*\\.dll\\.a$")
                set(OPENSSL_SSL_LIB ${lib})
            endif()
        endforeach()
        
        if(OPENSSL_CRYPTO_LIB AND OPENSSL_SSL_LIB)
            target_link_libraries(cton-sdk-core PRIVATE 
                ${OPENSSL_CRYPTO_LIB}
                ${OPENSSL_SSL_LIB})
            target_compile_definitions(cton-sdk-core PRIVATE OPENSSL_AVAILABLE=1)
            message(STATUS "Using OpenSSL import libraries: ${OPENSSL_CRYPTO_LIB}, ${OPENSSL_SSL_LIB}")
            
            # Also set for test executables
            target_compile_definitions(crypto_test PRIVATE OPENSSL_AVAILABLE=1)
            target_compile_definitions(ed25519_test PRIVATE OPENSSL_AVAILABLE=1)
            target_compile_definitions(crypto_algorithms_test PRIVATE OPENSSL_AVAILABLE=1)
            target_compile_definitions(crypto_security_test PRIVATE OPENSSL_AVAILABLE=1)
        else()
            message(WARNING "Could not find required OpenSSL import libraries in ${OPENSSL_ROOT_DIR}")
        endif()
    else()
        # Try to find system OpenSSL
        find_package(OpenSSL)
        if(OpenSSL_FOUND)
            target_link_libraries(cton-sdk-core PRIVATE OpenSSL::Crypto OpenSSL::SSL)
            target_compile_definitions(cton-sdk-core PRIVATE OPENSSL_AVAILABLE=1)
            message(STATUS "Using system OpenSSL libraries")
            
            # Also set for test executables
            target_compile_definitions(crypto_test PRIVATE OPENSSL_AVAILABLE=1)
            target_compile_definitions(ed25519_test PRIVATE OPENSSL_AVAILABLE=1)
            target_compile_definitions(crypto_algorithms_test PRIVATE OPENSSL_AVAILABLE=1)
            target_compile_definitions(crypto_security_test PRIVATE OPENSSL_AVAILABLE=1)
        else()
            message(FATAL_ERROR "OpenSSL libraries not found. OpenSSL is required for cryptographic operations.")
        endif()
    endif()
endif()

target_include_directories(cton-sdk-test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(cton-sdk-demo PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
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
    ${OPENSSL_INCLUDE_DIR}
)

target_include_directories(boc_test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(native_interface_test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(comprehensive_test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(ed25519_test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OPENSSL_INCLUDE_DIR}
)

target_include_directories(boc_compliance_test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(mnemonic_test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OPENSSL_INCLUDE_DIR}
)

target_include_directories(bip39_compliance_test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OPENSSL_INCLUDE_DIR}
)

target_include_directories(crypto_algorithms_test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OPENSSL_INCLUDE_DIR}
)

target_include_directories(boc_performance_test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(benchmark_test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(integration_test PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Compiler options
target_compile_options(cton-sdk-core PRIVATE 
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /wd4100 /wd4996 /wd4267>  # Disable various warnings
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

# Copy test runner script to build directory
configure_file(test/run_all_tests.bat run_all_tests.bat COPYONLY)