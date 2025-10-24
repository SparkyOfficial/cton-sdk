// Ed25519Test.cpp - тестування Ed25519 криптографії
// Author: Андрій Будильников (Sparky)
// Ed25519 cryptography testing
// Тестирование криптографии Ed25519

#include "../include/Crypto.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <string>

using namespace cton;

int main() {
    std::cout << "Testing Ed25519 cryptography functionality..." << std::endl;
    
    try {
        // Генеруємо приватний ключ
        // Generate private key
        // Генерируем приватный ключ
        std::cout << "1. Generating private key..." << std::endl;
        PrivateKey privateKey = PrivateKey::generate();
        
        // Отримуємо дані ключа
        // Get key data
        // Получаем данные ключа
        auto keyData = privateKey.getData();
        assert(keyData.size() == 32);
        std::cout << "   Private key size: " << keyData.size() << " bytes" << std::endl;
        
        // Отримуємо публічний ключ
        // Get public key
        // Получаем публичный ключ
        std::cout << "2. Deriving public key..." << std::endl;
        PublicKey publicKey = privateKey.getPublicKey();
        auto pubKeyData = publicKey.getData();
        assert(pubKeyData.size() == 32);
        std::cout << "   Public key size: " << pubKeyData.size() << " bytes" << std::endl;
        
        // Створюємо повідомлення для підпису
        // Create message to sign
        // Создаем сообщение для подписи
        std::cout << "3. Creating message to sign..." << std::endl;
        std::string messageStr = "Hello, TON!";
        std::vector<uint8_t> message(messageStr.begin(), messageStr.end());
        std::cout << "   Message: " << messageStr << std::endl;
        std::cout << "   Message size: " << message.size() << " bytes" << std::endl;
        
        // Підписуємо повідомлення
        // Sign message
        // Подписываем сообщение
        std::cout << "4. Signing message..." << std::endl;
        std::vector<uint8_t> signature = Crypto::sign(privateKey, message);
        assert(signature.size() == 64);
        std::cout << "   Signature size: " << signature.size() << " bytes" << std::endl;
        
        // Перевіряємо підпис
        // Verify signature
        // Проверяем подпись
        std::cout << "5. Verifying signature..." << std::endl;
        bool isValid = Crypto::verify(publicKey, message, signature);
        assert(isValid);
        std::cout << "   Signature verification: " << (isValid ? "VALID" : "INVALID") << std::endl;
        
        // Перевіряємо підпис з іншим повідомленням
        // Verify signature with different message
        // Проверяем подпись с другим сообщением
        std::cout << "6. Verifying signature with different message..." << std::endl;
        std::string differentMessageStr = "Hello, TON World!";
        std::vector<uint8_t> differentMessage(differentMessageStr.begin(), differentMessageStr.end());
        bool isInvalid = Crypto::verify(publicKey, differentMessage, signature);
        std::cout << "   Signature verification with different message: " << (isInvalid ? "VALID" : "INVALID") << std::endl;
        
        std::cout << "\nEd25519 test completed successfully!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in Ed25519 test: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error in Ed25519 test" << std::endl;
        return 1;
    }
}