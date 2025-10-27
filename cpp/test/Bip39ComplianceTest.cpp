// Bip39ComplianceTest.cpp - тестування BIP-39 сумісності
// Author: Андрій Будильников (Sparky)
// BIP-39 compliance testing
// Тестирование совместимости BIP-39

#include "../include/Mnemonic.h"
#include "../include/Crypto.h"
#include <iostream>
#include <cassert>
#include <vector>

using namespace cton;

int main() {
    std::cout << "Testing BIP-39 compliance..." << std::endl;
    
    try {
        // Тест 1: Перевірка розміру wordlist
        // Test 1: Check wordlist size
        // Тест 1: Проверка размера wordlist
        std::cout << "1. Checking wordlist size..." << std::endl;
        const auto& wordlist = Mnemonic::getWordlist();
        assert(wordlist.size() == 2048);
        std::cout << "   Wordlist size: " << wordlist.size() << " words (expected: 2048)" << std::endl;
        
        // Тест 2: Генерація мнемонічної фрази
        // Test 2: Generate mnemonic phrase
        // Тест 2: Генерация мнемонической фразы
        std::cout << "2. Generating 24-word mnemonic..." << std::endl;
        auto mnemonic24 = Mnemonic::generate(24);
        assert(mnemonic24.size() == 24);
        std::cout << "   24-word mnemonic generated: " << mnemonic24.size() << " words" << std::endl;
        
        // Тест 3: Генерація 12-слівної мнемоніки
        // Test 3: Generate 12-word mnemonic
        // Тест 3: Генерация 12-словной мнемоники
        std::cout << "3. Generating 12-word mnemonic..." << std::endl;
        auto mnemonic12 = Mnemonic::generate(12);
        assert(mnemonic12.size() == 12);
        std::cout << "   12-word mnemonic generated: " << mnemonic12.size() << " words" << std::endl;
        
        // Тест 4: Генерація 18-слівної мнемоніки
        // Test 4: Generate 18-word mnemonic
        // Тест 4: Генерация 18-словной мнемоники
        std::cout << "4. Generating 18-word mnemonic..." << std::endl;
        auto mnemonic18 = Mnemonic::generate(18);
        assert(mnemonic18.size() == 18);
        std::cout << "   18-word mnemonic generated: " << mnemonic18.size() << " words" << std::endl;
        
        // Тест 5: Перевірка валідності
        // Test 5: Validate mnemonic
        // Тест 5: Проверка валидности
        std::cout << "5. Validating mnemonics..." << std::endl;
        bool valid24 = Mnemonic::isValid(mnemonic24);
        bool valid12 = Mnemonic::isValid(mnemonic12);
        bool valid18 = Mnemonic::isValid(mnemonic18);
        assert(valid24);
        assert(valid12);
        assert(valid18);
        std::cout << "   All mnemonics are valid" << std::endl;
        
        // Тест 6: Конвертація мнемоніки в seed
        // Test 6: Convert mnemonic to seed
        // Тест 6: Конвертация мнемоники в сид
        std::cout << "6. Converting mnemonic to seed..." << std::endl;
        auto seed = Mnemonic::toSeed(mnemonic24);
        assert(seed.size() == 64); // BIP-39 seeds are 512 bits (64 bytes)
        std::cout << "   Seed generated: " << seed.size() << " bytes (expected: 64)" << std::endl;
        
        // Тест 7: Конвертація мнемоніки в приватний ключ через Crypto
        // Test 7: Convert mnemonic to private key via Crypto
        // Тест 7: Конвертация мнемоники в приватный ключ через Crypto
        std::cout << "7. Converting mnemonic to private key..." << std::endl;
        auto privateKey = Crypto::mnemonicToPrivateKey(mnemonic24);
        auto keyData = privateKey.getData();
        assert(keyData.size() == 32); // Ed25519 private keys are 32 bytes
        std::cout << "   Private key generated from mnemonic: " << keyData.size() << " bytes" << std::endl;
        
        // Тест 8: Отримання публічного ключа
        // Test 8: Get public key
        // Тест 8: Получение публичного ключа
        std::cout << "8. Deriving public key..." << std::endl;
        auto publicKey = privateKey.getPublicKey();
        auto pubKeyData = publicKey.getData();
        assert(pubKeyData.size() == 32); // Ed25519 public keys are 32 bytes
        std::cout << "   Public key derived: " << pubKeyData.size() << " bytes" << std::endl;
        
        // Тест 9: Підпис повідомлення
        // Test 9: Sign message
        // Тест 9: Подпись сообщения
        std::cout << "9. Signing message..." << std::endl;
        std::vector<uint8_t> message = {0x01, 0x02, 0x03, 0x04};
        auto signature = Crypto::sign(privateKey, message);
        assert(signature.size() == 64); // Ed25519 signatures are 64 bytes
        std::cout << "   Message signed: " << signature.size() << " bytes signature" << std::endl;
        
        // Тест 10: Перевірка підпису
        // Test 10: Verify signature
        // Тест 10: Проверка подписи
        std::cout << "10. Verifying signature..." << std::endl;
        bool verified = Crypto::verify(publicKey, message, signature);
        assert(verified);
        std::cout << "   Signature verified: " << (verified ? "valid" : "invalid") << std::endl;
        
        std::cout << "\nBIP-39 compliance test completed successfully!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in BIP-39 test: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error in BIP-39 test" << std::endl;
        return 1;
    }
}