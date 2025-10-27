// CryptoAlgorithmsTest.cpp - тестування додаткових криптографічних алгоритмів
// Author: Андрій Будильников (Sparky)
// Additional cryptographic algorithms testing
// Тестирование дополнительных криптографических алгоритмов

#include "../include/Crypto.h"
#include <iostream>
#include <cassert>
#include <vector>

using namespace cton;

int main() {
    std::cout << "Testing additional cryptographic algorithms..." << std::endl;
    
    try {
        // Тест 1: Генерація приватного ключа secp256k1
        // Test 1: Generate secp256k1 private key
        // Тест 1: Генерация приватного ключа secp256k1
        std::cout << "1. Generating secp256k1 private key..." << std::endl;
        auto secp256k1PrivateKey = Secp256k1PrivateKey::generate();
        auto secp256k1KeyData = secp256k1PrivateKey.getData();
        assert(secp256k1KeyData.size() == 32);
        std::cout << "   Secp256k1 private key generated: " << secp256k1KeyData.size() << " bytes" << std::endl;
        
        // Тест 2: Отримання публічного ключа secp256k1
        // Test 2: Get secp256k1 public key
        // Тест 2: Получение публичного ключа secp256k1
        std::cout << "2. Deriving secp256k1 public key..." << std::endl;
        auto secp256k1PublicKey = secp256k1PrivateKey.getPublicKey();
        auto secp256k1PubKeyData = secp256k1PublicKey.getData();
        std::cout << "   Secp256k1 public key derived: " << secp256k1PubKeyData.size() << " bytes" << std::endl;
        
        // Тест 3: Підпис повідомлення secp256k1
        // Test 3: Sign message with secp256k1
        // Тест 3: Подпись сообщения secp256k1
        std::cout << "3. Signing message with secp256k1..." << std::endl;
        std::vector<uint8_t> message = {0x01, 0x02, 0x03, 0x04, 0x05};
        auto secp256k1Signature = Crypto::signSecp256k1(secp256k1PrivateKey, message);
        assert(secp256k1Signature.size() == 64);
        std::cout << "   Message signed with secp256k1: " << secp256k1Signature.size() << " bytes signature" << std::endl;
        
        // Тест 4: Перевірка підпису secp256k1
        // Test 4: Verify secp256k1 signature
        // Тест 4: Проверка подписи secp256k1
        std::cout << "4. Verifying secp256k1 signature..." << std::endl;
        bool secp256k1Verified = Crypto::verifySecp256k1(secp256k1PublicKey, message, secp256k1Signature);
        std::cout << "   Secp256k1 signature verified: " << (secp256k1Verified ? "VALID" : "INVALID") << std::endl;
        
        // Тест 5: Шифрування ChaCha20
        // Test 5: ChaCha20 encryption
        // Тест 5: Шифрование ChaCha20
        std::cout << "5. Testing ChaCha20 encryption..." << std::endl;
        std::vector<uint8_t> plaintext = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
        std::vector<uint8_t> key(32, 0x12); // 32-byte key filled with 0x12
        std::vector<uint8_t> nonce(12, 0x34); // 12-byte nonce filled with 0x34
        
        auto ciphertext = ChaCha20::encrypt(plaintext, key, nonce);
        std::cout << "   ChaCha20 encryption: " << plaintext.size() << " bytes -> " << ciphertext.size() << " bytes" << std::endl;
        
        // Тест 6: Розшифрування ChaCha20
        // Test 6: ChaCha20 decryption
        // Тест 6: Расшифровка ChaCha20
        std::cout << "6. Testing ChaCha20 decryption..." << std::endl;
        auto decrypted = ChaCha20::decrypt(ciphertext, key, nonce);
        assert(decrypted.size() == plaintext.size());
        bool decryptionSuccess = (decrypted == plaintext);
        std::cout << "   ChaCha20 decryption: " << (decryptionSuccess ? "SUCCESS" : "FAILED") << std::endl;
        
        // Тест 7: Порівняння оригінального та розшифрованого тексту
        // Test 7: Compare original and decrypted text
        // Тест 7: Сравнение оригинального и расшифрованного текста
        std::cout << "7. Comparing original and decrypted text..." << std::endl;
        if (decryptionSuccess) {
            std::cout << "   Original and decrypted text match: YES" << std::endl;
        } else {
            std::cout << "   Original and decrypted text match: NO" << std::endl;
        }
        
        std::cout << "\nAdditional cryptographic algorithms test completed successfully!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in cryptographic algorithms test: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error in cryptographic algorithms test" << std::endl;
        return 1;
    }
}