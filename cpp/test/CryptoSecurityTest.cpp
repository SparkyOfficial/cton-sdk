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

int main() {
    std::cout << "CTON-SDK Crypto Security Test" << std::endl;
    std::cout << "=============================" << std::endl;
    
    try {
        // 1. Тест генерації приватного ключа
        // 1. Private key generation test
        // 1. Тест генерации приватного ключа
        std::cout << "\n1. Private Key Generation Test:" << std::endl;
        PrivateKey privateKey = PrivateKey::generate();
        std::vector<uint8_t> keyData = privateKey.getData();
        ASSERT_EQUAL(keyData.size(), 32, "Private key should be 32 bytes");
        std::cout << "   ✓ Private key generated successfully" << std::endl;
        
        // 2. Тест отримання публічного ключа
        // 2. Public key derivation test
        // 2. Тест получения публичного ключа
        std::cout << "\n2. Public Key Derivation Test:" << std::endl;
        PublicKey publicKey = privateKey.getPublicKey();
        std::vector<uint8_t> pubKeyData = publicKey.getData();
        ASSERT_EQUAL(pubKeyData.size(), 32, "Public key should be 32 bytes");
        std::cout << "   ✓ Public key derived successfully" << std::endl;
        
        // 3. Тест підпису та перевірки
        // 3. Signature and verification test
        // 3. Тест подписи и проверки
        std::cout << "\n3. Signature and Verification Test:" << std::endl;
        std::string message = "Hello, TON!";
        std::vector<uint8_t> messageBytes(message.begin(), message.end());
        
        // Створюємо підпис
        // Create signature
        // Создаем подпись
        std::vector<uint8_t> signature = Crypto::sign(privateKey, messageBytes);
        ASSERT_EQUAL(signature.size(), 64, "Signature should be 64 bytes");
        std::cout << "   ✓ Signature created successfully" << std::endl;
        
        // Перевіряємо підпис
        // Verify signature
        // Проверяем подпись
        bool isValid = Crypto::verify(publicKey, messageBytes, signature);
        ASSERT_TRUE(isValid, "Signature should be valid");
        std::cout << "   ✓ Signature verified successfully" << std::endl;
        
        // 4. Тест невалідного підпису
        // 4. Invalid signature test
        // 4. Тест невалидной подписи
        std::cout << "\n4. Invalid Signature Test:" << std::endl;
        std::string wrongMessage = "Wrong message";
        std::vector<uint8_t> wrongMessageBytes(wrongMessage.begin(), wrongMessage.end());
        bool isInvalid = Crypto::verify(publicKey, wrongMessageBytes, signature);
        ASSERT_FALSE(isInvalid, "Signature should be invalid for wrong message");
        std::cout << "   ✓ Invalid signature correctly rejected" << std::endl;
        
        // 5. Тест secp256k1
        // 5. Secp256k1 test
        // 5. Тест secp256k1
        std::cout << "\n5. Secp256k1 Test:" << std::endl;
        Secp256k1PrivateKey secpPrivateKey = Secp256k1PrivateKey::generate();
        std::vector<uint8_t> secpKeyData = secpPrivateKey.getData();
        ASSERT_EQUAL(secpKeyData.size(), 32, "Secp256k1 private key should be 32 bytes");
        std::cout << "   ✓ Secp256k1 private key generated successfully" << std::endl;
        
        Secp256k1PublicKey secpPublicKey = secpPrivateKey.getPublicKey();
        std::vector<uint8_t> secpPubKeyData = secpPublicKey.getData();
        ASSERT_TRUE(secpPubKeyData.size() == 33 || secpPubKeyData.size() == 65, 
                   "Secp256k1 public key should be 33 or 65 bytes");
        std::cout << "   ✓ Secp256k1 public key derived successfully" << std::endl;
        
        // Створюємо підпис secp256k1
        // Create secp256k1 signature
        // Создаем подпись secp256k1
        std::vector<uint8_t> secpSignature = Crypto::signSecp256k1(secpPrivateKey, messageBytes);
        ASSERT_EQUAL(secpSignature.size(), 64, "Secp256k1 signature should be 64 bytes");
        std::cout << "   ✓ Secp256k1 signature created successfully" << std::endl;
        
        // Перевіряємо підпис secp256k1
        // Verify secp256k1 signature
        // Проверяем подпись secp256k1
        bool secpIsValid = Crypto::verifySecp256k1(secpPublicKey, messageBytes, secpSignature);
        ASSERT_TRUE(secpIsValid, "Secp256k1 signature should be valid");
        std::cout << "   ✓ Secp256k1 signature verified successfully" << std::endl;
        
        std::cout << "\n🎉 All crypto security tests passed!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}