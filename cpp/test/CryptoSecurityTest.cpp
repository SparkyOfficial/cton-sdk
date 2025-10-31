// CryptoSecurityTest.cpp - тест безпеки криптографічних функцій
// Author: Андрій Будильников (Sparky)
// Crypto security test
// Тест безопасности криптографических функций

#include "../include/Crypto.h"
#include "TestFramework.h"
#include <iostream>
#include <vector>
#include <string>

using namespace cton;

TEST(PrivateKeyGeneration) {
    PrivateKey privateKey = PrivateKey::generate();
    std::vector<uint8_t> keyData = privateKey.getData();
    ASSERT_EQUAL(keyData.size(), 32);
}

TEST(PublicKeyDerivation) {
    PrivateKey privateKey = PrivateKey::generate();
    PublicKey publicKey = privateKey.getPublicKey();
    std::vector<uint8_t> pubKeyData = publicKey.getData();
    ASSERT_EQUAL(pubKeyData.size(), 32);
}

TEST(SignatureAndVerification) {
    PrivateKey privateKey = PrivateKey::generate();
    PublicKey publicKey = privateKey.getPublicKey();
    
    std::string message = "Hello, TON!";
    std::vector<uint8_t> messageBytes(message.begin(), message.end());
    
    // Створюємо підпис
    // Create signature
    // Создаем подпись
    std::vector<uint8_t> signature = Crypto::sign(privateKey, messageBytes);
    ASSERT_EQUAL(signature.size(), 64);
    
    // Перевіряємо підпис
    // Verify signature
    // Проверяем подпись
    bool isValid = Crypto::verify(publicKey, messageBytes, signature);
    ASSERT_TRUE(isValid);
}

TEST(InvalidSignature) {
    PrivateKey privateKey = PrivateKey::generate();
    PublicKey publicKey = privateKey.getPublicKey();
    
    std::string message = "Hello, TON!";
    std::vector<uint8_t> messageBytes(message.begin(), message.end());
    
    // Створюємо підпис
    // Create signature
    // Создаем подпись
    std::vector<uint8_t> signature = Crypto::sign(privateKey, messageBytes);
    
    // Тест невалідного підпису
    // Invalid signature test
    // Тест невалидной подписи
    std::string wrongMessage = "Wrong message";
    std::vector<uint8_t> wrongMessageBytes(wrongMessage.begin(), wrongMessage.end());
    bool isInvalid = Crypto::verify(publicKey, wrongMessageBytes, signature);
    ASSERT_FALSE(isInvalid);
}

TEST(Secp256k1) {
    // Тест secp256k1
    // Secp256k1 test
    // Тест secp256k1
    Secp256k1PrivateKey secpPrivateKey = Secp256k1PrivateKey::generate();
    std::vector<uint8_t> secpKeyData = secpPrivateKey.getData();
    ASSERT_EQUAL(secpKeyData.size(), 32);
    
    Secp256k1PublicKey secpPublicKey = secpPrivateKey.getPublicKey();
    std::vector<uint8_t> secpPubKeyData = secpPublicKey.getData();
    ASSERT_TRUE(secpPubKeyData.size() == 33 || secpPubKeyData.size() == 65);
    
    std::string message = "Hello, TON!";
    std::vector<uint8_t> messageBytes(message.begin(), message.end());
    
    // Створюємо підпис secp256k1
    // Create secp256k1 signature
    // Создаем подпись secp256k1
    std::vector<uint8_t> secpSignature = Crypto::signSecp256k1(secpPrivateKey, messageBytes);
    ASSERT_EQUAL(secpSignature.size(), 64);
    
    // Перевіряємо підпис secp256k1
    // Verify secp256k1 signature
    // Проверяем подпись secp256k1
    bool secpIsValid = Crypto::verifySecp256k1(secpPublicKey, messageBytes, secpSignature);
    ASSERT_TRUE(secpIsValid);
}

int main() {
    std::cout << "CTON-SDK Crypto Security Test" << std::endl;
    std::cout << "=============================" << std::endl;
    
    int result = RUN_ALL_TESTS();
    
    if (result == 0) {
        std::cout << "\n🎉 All crypto security tests passed!" << std::endl;
    } else {
        std::cout << "\n❌ Some tests failed!" << std::endl;
    }
    
    return result;
}