// MnemonicTest.cpp - тестування мнемонічних фраз
// Author: Андрій Будильников (Sparky)
// Mnemonic phrase testing
// Тестирование мнемонических фраз

#include "../include/Mnemonic.h"
#include "../include/Crypto.h"
#include <iostream>
#include <cassert>
#include <vector>

using namespace cton;

int main() {
    std::cout << "Testing mnemonic functionality..." << std::endl;
    
    try {
        // Тест 1: Генерація мнемонічної фрази
        // Test 1: Generate mnemonic phrase
        // Тест 1: Генерация мнемонической фразы
        std::cout << "1. Generating 24-word mnemonic..." << std::endl;
        auto mnemonic24 = Mnemonic::generate(24);
        assert(mnemonic24.size() == 24);
        std::cout << "   24-word mnemonic generated: " << mnemonic24.size() << " words" << std::endl;
        
        // Тест 2: Генерація 12-слівної мнемоніки
        // Test 2: Generate 12-word mnemonic
        // Тест 2: Генерация 12-словной мнемоники
        std::cout << "2. Generating 12-word mnemonic..." << std::endl;
        auto mnemonic12 = Mnemonic::generate(12);
        assert(mnemonic12.size() == 12);
        std::cout << "   12-word mnemonic generated: " << mnemonic12.size() << " words" << std::endl;
        
        // Тест 3: Генерація 18-слівної мнемоніки
        // Test 3: Generate 18-word mnemonic
        // Тест 3: Генерация 18-словной мнемоники
        std::cout << "3. Generating 18-word mnemonic..." << std::endl;
        auto mnemonic18 = Mnemonic::generate(18);
        assert(mnemonic18.size() == 18);
        std::cout << "   18-word mnemonic generated: " << mnemonic18.size() << " words" << std::endl;
        
        // Тест 4: Перевірка валідності
        // Test 4: Validate mnemonic
        // Тест 4: Проверка валидности
        std::cout << "4. Validating mnemonics..." << std::endl;
        bool valid24 = Mnemonic::isValid(mnemonic24);
        bool valid12 = Mnemonic::isValid(mnemonic12);
        bool valid18 = Mnemonic::isValid(mnemonic18);
        assert(valid24);
        assert(valid12);
        assert(valid18);
        std::cout << "   All mnemonics are valid" << std::endl;
        
        // Тест 5: Отримання wordlist
        // Test 5: Get wordlist
        // Тест 5: Получение wordlist
        std::cout << "5. Getting wordlist..." << std::endl;
        const auto& wordlist = Mnemonic::getWordlist();
        assert(!wordlist.empty());
        std::cout << "   Wordlist size: " << wordlist.size() << " words" << std::endl;
        
        // Тест 6: Інтеграція з Crypto
        // Test 6: Integration with Crypto
        // Тест 6: Интеграция с Crypto
        std::cout << "6. Testing Crypto integration..." << std::endl;
        auto cryptoMnemonic = Crypto::generateMnemonic();
        assert(cryptoMnemonic.size() == 24);
        std::cout << "   Crypto mnemonic generated: " << cryptoMnemonic.size() << " words" << std::endl;
        
        // Тест 7: Конвертація мнемоніки в приватний ключ
        // Test 7: Convert mnemonic to private key
        // Тест 7: Конвертация мнемоники в приватный ключ
        std::cout << "7. Converting mnemonic to private key..." << std::endl;
        auto privateKey = Crypto::mnemonicToPrivateKey(cryptoMnemonic);
        auto keyData = privateKey.getData();
        assert(keyData.size() == 32);
        std::cout << "   Private key generated from mnemonic: " << keyData.size() << " bytes" << std::endl;
        
        std::cout << "\nMnemonic functionality test completed successfully!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in mnemonic test: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error in mnemonic test" << std::endl;
        return 1;
    }
}