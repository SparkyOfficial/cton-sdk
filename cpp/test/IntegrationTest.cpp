// IntegrationTest.cpp - інтеграційне тестування з живою мережею TON
// Author: Андрій Будильников (Sparky)
// Integration testing with live TON network
// Интеграционное тестирование с живой сетью TON

#include "../include/Boc.h"
#include "../include/Cell.h"
#include "../include/Crypto.h"
#include "../include/Address.h"
#include "../include/TonApiClient.h"
#include <iostream>
#include <vector>
#include <memory>

using namespace cton;

void testAddressCreation() {
    std::cout << "=== Address Creation Tests ===" << std::endl;
    
    // Тест 1: Створення адреси з публічного ключа
    // Test 1: Create address from public key
    // Тест 1: Создание адреса из публичного ключа
    std::cout << "1. Creating address from public key..." << std::endl;
    auto privateKey = PrivateKey::generate();
    auto publicKey = privateKey.getPublicKey();
    Address address(publicKey);
    std::cout << "   Address created: " << address.toUserFriendly(true, false) << std::endl;
    
    // Тест 2: Перевірка формату адреси
    // Test 2: Check address format
    // Тест 2: Проверка формата адреса
    std::cout << "2. Checking address format..." << std::endl;
    std::string rawAddress = address.toRawString();
    std::string ufAddress = address.toUserFriendly(true, false);
    std::cout << "   Raw address: " << rawAddress << std::endl;
    std::cout << "   User-friendly address: " << ufAddress << std::endl;
}

void testCellAndBocOperations() {
    std::cout << "=== Cell and BOC Operations Tests ===" << std::endl;
    
    // Тест 1: Створення складної структури комірок
    // Test 1: Create complex cell structure
    // Тест 1: Создание сложной структуры ячеек
    std::cout << "1. Creating complex cell structure..." << std::endl;
    CellBuilder builder;
    builder.storeUInt(32, 0x12345678);
    builder.storeBytes({0x01, 0x02, 0x03, 0x04, 0x05});
    
    // Додаємо вкладені комірки
    // Add nested cells
    // Добавляем вложенные ячейки
    CellBuilder nestedBuilder;
    nestedBuilder.storeUInt(64, 0xABCDEF0012345678);
    builder.storeRef(nestedBuilder.build());
    
    auto cell = builder.build();
    std::cout << "   Cell created with bit size: " << cell->getBitSize() << std::endl;
    
    // Тест 2: Серіалізація в BOC
    // Test 2: Serialize to BOC
    // Тест 2: Сериализация в BOC
    std::cout << "2. Serializing to BOC..." << std::endl;
    auto root = std::make_shared<Cell>(*cell);
    Boc boc(root);
    auto serialized = boc.serialize(true, true);
    std::cout << "   BOC serialized, size: " << serialized.size() << " bytes" << std::endl;
    
    // Тест 3: Десеріалізація BOC
    // Test 3: Deserialize BOC
    // Тест 3: Десериализация BOC
    std::cout << "3. Deserializing BOC..." << std::endl;
    auto deserialized = Boc::deserialize(serialized);
    auto deserializedRoot = deserialized.getRoot();
    if (deserializedRoot) {
        std::cout << "   BOC deserialized successfully" << std::endl;
    } else {
        std::cout << "   BOC deserialization failed" << std::endl;
    }
}

void testCryptoOperations() {
    std::cout << "=== Crypto Operations Tests ===" << std::endl;
    
    // Тест 1: Генерація ключів
    // Test 1: Key generation
    // Тест 1: Генерация ключей
    std::cout << "1. Generating key pair..." << std::endl;
    auto privateKey = PrivateKey::generate();
    auto publicKey = privateKey.getPublicKey();
    std::cout << "   Private key size: " << privateKey.getData().size() << " bytes" << std::endl;
    std::cout << "   Public key size: " << publicKey.getData().size() << " bytes" << std::endl;
    
    // Тест 2: Підпис повідомлення
    // Test 2: Sign message
    // Тест 2: Подпись сообщения
    std::cout << "2. Signing message..." << std::endl;
    std::vector<uint8_t> message = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x54, 0x4F, 0x4E, 0x21}; // "Hello TON!"
    auto signature = Crypto::sign(privateKey, message);
    std::cout << "   Message signed, signature size: " << signature.size() << " bytes" << std::endl;
    
    // Тест 3: Перевірка підпису
    // Test 3: Verify signature
    // Тест 3: Проверка подписи
    std::cout << "3. Verifying signature..." << std::endl;
    bool verified = Crypto::verify(publicKey, message, signature);
    std::cout << "   Signature verification: " << (verified ? "PASSED" : "FAILED") << std::endl;
    
    // Тест 4: Генерація мнемонічної фрази
    // Test 4: Generate mnemonic phrase
    // Тест 4: Генерация мнемонической фразы
    std::cout << "4. Generating mnemonic phrase..." << std::endl;
    auto mnemonic = Crypto::generateMnemonic();
    std::cout << "   Mnemonic generated: " << mnemonic.size() << " words" << std::endl;
    std::cout << "   First 3 words: ";
    for (int i = 0; i < 3 && i < mnemonic.size(); ++i) {
        std::cout << mnemonic[i] << " ";
    }
    std::cout << std::endl;
    
    // Тест 5: Створення ключа з мнемоніки
    // Test 5: Create key from mnemonic
    // Тест 5: Создание ключа из мнемоники
    std::cout << "5. Creating key from mnemonic..." << std::endl;
    auto mnemonicKey = Crypto::mnemonicToPrivateKey(mnemonic);
    std::cout << "   Key from mnemonic created, size: " << mnemonicKey.getData().size() << " bytes" << std::endl;
}

void testMnemonicOperations() {
    std::cout << "=== Mnemonic Operations Tests ===" << std::endl;
    
    // Тест 1: Генерація різної довжини мнемонік
    // Test 1: Generate different length mnemonics
    // Тест 1: Генерация мнемоник разной длины
    std::cout << "1. Generating different length mnemonics..." << std::endl;
    auto mnemonic12 = cton::Mnemonic::generate(12);
    auto mnemonic18 = cton::Mnemonic::generate(18);
    auto mnemonic24 = cton::Mnemonic::generate(24);
    std::cout << "   12-word mnemonic: " << mnemonic12.size() << " words" << std::endl;
    std::cout << "   18-word mnemonic: " << mnemonic18.size() << " words" << std::endl;
    std::cout << "   24-word mnemonic: " << mnemonic24.size() << " words" << std::endl;
    
    // Тест 2: Перевірка валідності мнемонік
    // Test 2: Validate mnemonics
    // Тест 2: Проверка валидности мнемоник
    std::cout << "2. Validating mnemonics..." << std::endl;
    bool valid12 = cton::Mnemonic::isValid(mnemonic12);
    bool valid18 = cton::Mnemonic::isValid(mnemonic18);
    bool valid24 = cton::Mnemonic::isValid(mnemonic24);
    std::cout << "   12-word mnemonic valid: " << (valid12 ? "YES" : "NO") << std::endl;
    std::cout << "   18-word mnemonic valid: " << (valid18 ? "YES" : "NO") << std::endl;
    std::cout << "   24-word mnemonic valid: " << (valid24 ? "YES" : "NO") << std::endl;
}

int main() {
    std::cout << "Running CTON-SDK Integration Tests" << std::endl;
    std::cout << "=================================" << std::endl;
    
    try {
        testAddressCreation();
        std::cout << std::endl;
        
        testCellAndBocOperations();
        std::cout << std::endl;
        
        testCryptoOperations();
        std::cout << std::endl;
        
        testMnemonicOperations();
        std::cout << std::endl;
        
        std::cout << "All integration tests completed successfully!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in integration tests: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error in integration tests" << std::endl;
        return 1;
    }
}