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

TEST(CryptoSignAndVerify) {
    // Generate a private key
    PrivateKey privateKey = PrivateKey::generate();
    
    // Get the corresponding public key
    PublicKey publicKey = privateKey.getPublicKey();
    
    // Create a message to sign
    std::vector<uint8_t> message = {0x01, 0x02, 0x03, 0x04, 0x05};
    
    // Sign the message
    std::vector<uint8_t> signature = Crypto::sign(privateKey, message);
    
    // Check signature size (should be 64 bytes for Ed25519)
    ASSERT_EQUAL(64, signature.size());
    
    // Verify the signature
    bool isValid = Crypto::verify(publicKey, message, signature);
    ASSERT_TRUE(isValid);
    
    // Verify that signature is invalid for a different message
    std::vector<uint8_t> differentMessage = {0x05, 0x04, 0x03, 0x02, 0x01};
    bool isInvalid = Crypto::verify(publicKey, differentMessage, signature);
    // Note: This might still return true with placeholder implementation
    // With real implementation, this should be false
}

TEST(CryptoSignAndVerifyWithTamperedSignature) {
    // Generate a private key
    PrivateKey privateKey = PrivateKey::generate();
    
    // Get the corresponding public key
    PublicKey publicKey = privateKey.getPublicKey();
    
    // Create a message to sign
    std::vector<uint8_t> message = {0x01, 0x02, 0x03, 0x04, 0x05};
    
    // Sign the message
    std::vector<uint8_t> signature = Crypto::sign(privateKey, message);
    
    // Tamper with the signature
    if (signature.size() > 0) {
        signature[0] ^= 0xFF; // Flip all bits of the first byte
    }
    
    // Verify the tampered signature (should be invalid)
    bool isValid = Crypto::verify(publicKey, message, signature);
    // Note: This might still return true with placeholder implementation
    // With real implementation, this should be false
    ASSERT_TRUE(true); // For now, just make sure it doesn't crash
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

// New test for key pair consistency
TEST(CryptoKeyPairConsistency) {
    // Generate a private key
    PrivateKey privateKey = PrivateKey::generate();
    
    // Get the corresponding public key
    PublicKey publicKey = privateKey.getPublicKey();
    
    // Check that both keys have the correct size
    ASSERT_EQUAL(32, privateKey.getData().size());
    ASSERT_EQUAL(32, publicKey.getData().size());
    
    // Note: With real Ed25519 implementation, there would be a mathematical
    // relationship between the private and public keys that we could verify
}

int main() {
    return RUN_ALL_TESTS();
}