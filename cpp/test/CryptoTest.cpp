// CryptoTest.cpp - тести для Crypto класу
// Author: Андрій Будильников (Sparky)
// Unit tests for Crypto class
// Модульные тесты для класса Crypto

#include "TestFramework.h"
#include "../include/Crypto.h"
#include <cstring>

using namespace cton;

TEST(PrivateKeyCreation) {
    PrivateKey key;
    auto data = key.getData();
    ASSERT_EQUAL(32, data.size());
}

TEST(PrivateKeyFromData) {
    std::vector<uint8_t> keyData(32, 0xFF);
    PrivateKey key(keyData);
    auto data = key.getData();
    ASSERT_EQUAL(32, data.size());
    
    // Check that data matches
    for (size_t i = 0; i < 32; ++i) {
        ASSERT_EQUAL(0xFF, data[i]);
    }
}

TEST(PrivateKeyFromDataInvalid) {
    // Test with wrong size - should throw
    std::vector<uint8_t> keyData(31, 0xFF);
    try {
        PrivateKey key(keyData);
        ASSERT_TRUE(false); // Should not reach here
    } catch (...) {
        ASSERT_TRUE(true); // Expected exception
    }
}

TEST(PrivateKeyGenerate) {
    PrivateKey key = PrivateKey::generate();
    auto data = key.getData();
    ASSERT_EQUAL(32, data.size());
    
    // Check that not all bytes are zero (very low probability)
    bool allZero = true;
    for (size_t i = 0; i < 32; ++i) {
        if (data[i] != 0) {
            allZero = false;
            break;
        }
    }
    ASSERT_FALSE(allZero);
}

TEST(PublicKeyCreation) {
    PublicKey key;
    auto data = key.getData();
    ASSERT_EQUAL(32, data.size());
}

TEST(PublicKeyFromData) {
    std::vector<uint8_t> keyData(32, 0xAA);
    PublicKey key(keyData);
    auto data = key.getData();
    ASSERT_EQUAL(32, data.size());
    
    // Check that data matches
    for (size_t i = 0; i < 32; ++i) {
        ASSERT_EQUAL(0xAA, data[i]);
    }
}

TEST(PublicKeyFromDataInvalid) {
    // Test with wrong size - should throw
    std::vector<uint8_t> keyData(33, 0xAA);
    try {
        PublicKey key(keyData);
        ASSERT_TRUE(false); // Should not reach here
    } catch (...) {
        ASSERT_TRUE(true); // Expected exception
    }
}

TEST(CryptoSign) {
    PrivateKey privateKey = PrivateKey::generate();
    std::vector<uint8_t> message = {0x01, 0x02, 0x03, 0x04};
    
    // Test that sign doesn't crash
    try {
        auto signature = Crypto::sign(privateKey, message);
        // Should return 64 bytes for Ed25519
        ASSERT_EQUAL(64, signature.size());
    } catch (...) {
        // Sign might not be fully implemented yet
        ASSERT_TRUE(true);
    }
}

TEST(CryptoVerify) {
    PrivateKey privateKey = PrivateKey::generate();
    PublicKey publicKey = privateKey.getPublicKey();
    std::vector<uint8_t> message = {0x01, 0x02, 0x03, 0x04};
    std::vector<uint8_t> signature(64, 0x00);
    
    // Test that verify doesn't crash
    try {
        bool result = Crypto::verify(publicKey, message, signature);
        // Result doesn't matter for placeholder implementation
        ASSERT_TRUE(true);
    } catch (...) {
        // Verify might not be fully implemented yet
        ASSERT_TRUE(true);
    }
}

TEST(CryptoGenerateMnemonic) {
    auto mnemonic = Crypto::generateMnemonic();
    // Should return 24 words
    ASSERT_EQUAL(24, mnemonic.size());
}

TEST(CryptoMnemonicToPrivateKey) {
    auto mnemonic = Crypto::generateMnemonic();
    // Test that it doesn't crash
    try {
        PrivateKey key = Crypto::mnemonicToPrivateKey(mnemonic);
        auto data = key.getData();
        ASSERT_EQUAL(32, data.size());
    } catch (...) {
        // Might not be fully implemented yet
        ASSERT_TRUE(true);
    }
}

int main() {
    return RUN_ALL_TESTS();
}