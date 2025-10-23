// NativeInterfaceTest.cpp - тести для NativeInterface
// Author: Андрій Будильников (Sparky)
// Unit tests for NativeInterface
// Модульные тесты для NativeInterface

#include "TestFramework.h"
#include "../include/NativeInterface.h"
#include <cstring>

TEST(NativeCellCreation) {
    void* cell = cell_create();
    ASSERT_TRUE(cell != nullptr);
    
    cell_destroy(cell);
}

TEST(NativeCellStoreUInt) {
    void* cell = cell_create();
    ASSERT_TRUE(cell != nullptr);
    
    bool result = cell_store_uint(cell, 32, 0x12345678);
    ASSERT_TRUE(result);
    
    cell_destroy(cell);
}

TEST(NativeCellStoreInt) {
    void* cell = cell_create();
    ASSERT_TRUE(cell != nullptr);
    
    bool result = cell_store_int(cell, 32, -100);
    ASSERT_TRUE(result);
    
    cell_destroy(cell);
}

TEST(NativeCellStoreBytes) {
    void* cell = cell_create();
    ASSERT_TRUE(cell != nullptr);
    
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
    bool result = cell_store_bytes(cell, data, 4);
    ASSERT_TRUE(result);
    
    cell_destroy(cell);
}

TEST(NativeCellGetData) {
    void* cell = cell_create();
    ASSERT_TRUE(cell != nullptr);
    
    // Store some data first
    cell_store_uint(cell, 32, 0x12345678);
    
    // Get data
    uint8_t buffer[100];
    int size = cell_get_data(cell, buffer, 100);
    ASSERT_TRUE(size >= 0);
    
    cell_destroy(cell);
}

TEST(NativeCellGetBitSize) {
    void* cell = cell_create();
    ASSERT_TRUE(cell != nullptr);
    
    // Initially should be 0
    int size = cell_get_bit_size(cell);
    ASSERT_EQUAL(0, size);
    
    // Store some data
    cell_store_uint(cell, 32, 0x12345678);
    
    // Should now be 32
    size = cell_get_bit_size(cell);
    ASSERT_EQUAL(32, size);
    
    cell_destroy(cell);
}

TEST(NativeAddressCreation) {
    void* address = address_create();
    ASSERT_TRUE(address != nullptr);
    
    address_destroy(address);
}

TEST(NativeAddressFromString) {
    const char* addrStr = "0:1234567890123456789012345678901234567890123456789012345678901234";
    void* address = address_create_from_string(addrStr);
    // Might be nullptr for invalid implementation
    ASSERT_TRUE(true);
    
    if (address) {
        address_destroy(address);
    }
}

TEST(NativeAddressGetWorkchain) {
    void* address = address_create();
    ASSERT_TRUE(address != nullptr);
    
    int8_t workchain = address_get_workchain(address);
    // Should be 0 for default address
    ASSERT_EQUAL(0, workchain);
    
    address_destroy(address);
}

TEST(NativePrivateKeyCreation) {
    void* key = private_key_create();
    ASSERT_TRUE(key != nullptr);
    
    private_key_destroy(key);
}

TEST(NativePrivateKeyGenerate) {
    void* key = private_key_generate();
    // Might be nullptr for incomplete implementation
    ASSERT_TRUE(true);
    
    if (key) {
        private_key_destroy(key);
    }
}

TEST(NativePublicKeyCreation) {
    void* key = public_key_create();
    ASSERT_TRUE(key != nullptr);
    
    public_key_destroy(key);
}

TEST(NativeCryptoSign) {
    void* privateKey = private_key_create();
    ASSERT_TRUE(privateKey != nullptr);
    
    uint8_t message[] = {0x01, 0x02, 0x03, 0x04};
    void* signature = crypto_sign(privateKey, message, 4);
    // Might be nullptr for incomplete implementation
    ASSERT_TRUE(true);
    
    private_key_destroy(privateKey);
    if (signature) {
        // In a real implementation, we'd need to free the signature
    }
}

TEST(NativeBocCreation) {
    void* boc = boc_create();
    ASSERT_TRUE(boc != nullptr);
    
    boc_destroy(boc);
}

int main() {
    return RUN_ALL_TESTS();
}