// ComprehensiveTest.cpp - комплексне тестування CTON-SDK
// Author: Андрій Будильников (Sparky)
// Comprehensive testing of CTON-SDK functionality
// Комплексное тестирование CTON-SDK

#include "../include/Cell.h"
#include "../include/Address.h"
#include "../include/Crypto.h"
#include "../include/Boc.h"
#include <iostream>
#include <cassert>
#include <vector>

using namespace cton;

void testCellBasic() {
    std::cout << "Testing Cell basic functionality..." << std::endl;
    
    // Створюємо комірку
    // Create cell
    // Создаем ячейку
    Cell cell;
    
    // Додаємо дані
    // Add data
    // Добавляем данные
    cell.storeUInt(8, 0xFF);
    cell.storeInt(8, -1);
    cell.storeBytes({0x01, 0x02, 0x03});
    
    // Перевіряємо розмір
    // Check size
    // Проверяем размер
    assert(cell.getBitSize() > 0);
    
    // Перевіряємо кількість референсів
    // Check reference count
    // Проверяем количество ссылок
    auto refs = cell.getReferences();
    assert(refs.size() == 0);
    
    std::cout << "Cell basic functionality test passed!" << std::endl;
}

void testCellBuilder() {
    std::cout << "Testing CellBuilder functionality..." << std::endl;
    
    // Створюємо CellBuilder
    // Create CellBuilder
    // Создаем CellBuilder
    CellBuilder builder;
    
    // Додаємо різні типи даних
    // Add different data types
    // Добавляем разные типы данных
    builder.storeUInt(32, 0x12345678)
           .storeInt(64, -123456789LL)
           .storeBytes({0x01, 0x02, 0x03, 0x04});
    
    // Створюємо комірку
    // Create cell
    // Создаем ячейку
    auto cell = builder.build();
    
    // Перевіряємо
    // Check
    // Проверяем
    assert(cell->getBitSize() > 0);
    
    std::cout << "CellBuilder functionality test passed!" << std::endl;
}

void testAddress() {
    std::cout << "Testing Address functionality..." << std::endl;
    
    // Створюємо адресу з рядка
    // Create address from string
    // Создаем адрес из строки
    Address addr("0:1234567890123456789012345678901234567890123456789012345678901234");
    
    // Перевіряємо валідність
    // Check validity
    // Проверяем валидность
    assert(addr.isValid());
    
    // Перевіряємо workchain
    // Check workchain
    // Проверяем workchain
    assert(addr.getWorkchain() == 0);
    
    // Перетворюємо в raw формат
    // Convert to raw format
    // Преобразуем в raw формат
    std::string raw = addr.toRaw();
    assert(!raw.empty());
    
    // Перетворюємо в user-friendly формат
    // Convert to user-friendly format
    // Преобразуем в user-friendly формат
    std::string uf = addr.toUserFriendly(true, false);
    assert(!uf.empty());
    
    std::cout << "Address functionality test passed!" << std::endl;
}

void testCrypto() {
    std::cout << "Testing Crypto functionality..." << std::endl;
    
    // Генеруємо приватний ключ
    // Generate private key
    // Генерируем приватный ключ
    auto privateKey = PrivateKey::generate();
    
    // Отримуємо дані ключа
    // Get key data
    // Получаем данные ключа
    auto keyData = privateKey.getData();
    assert(keyData.size() == 32);
    
    // Отримуємо публічний ключ
    // Get public key
    // Получаем публичный ключ
    auto publicKey = privateKey.getPublicKey();
    auto pubKeyData = publicKey.getData();
    assert(pubKeyData.size() == 32);
    
    // Генеруємо мнемонічну фразу
    // Generate mnemonic phrase
    // Генерируем мнемоническую фразу
    auto mnemonic = Crypto::generateMnemonic();
    assert(mnemonic.size() == 24);
    
    // Створюємо ключ з мнемоніки
    // Create key from mnemonic
    // Создаем ключ из мнемоники
    auto mnemonicKey = Crypto::mnemonicToPrivateKey(mnemonic);
    auto mnemonicKeyData = mnemonicKey.getData();
    assert(mnemonicKeyData.size() == 32);
    
    std::cout << "Crypto functionality test passed!" << std::endl;
}

void testAdditionalCrypto() {
    std::cout << "Testing additional cryptographic algorithms..." << std::endl;
    
    // Генеруємо secp256k1 приватний ключ
    // Generate secp256k1 private key
    // Генерируем приватный ключ secp256k1
    auto secp256k1PrivateKey = Secp256k1PrivateKey::generate();
    
    // Отримуємо дані ключа
    // Get key data
    // Получаем данные ключа
    auto secp256k1KeyData = secp256k1PrivateKey.getData();
    assert(secp256k1KeyData.size() == 32);
    
    // Отримуємо публічний ключ
    // Get public key
    // Получаем публичный ключ
    auto secp256k1PublicKey = secp256k1PrivateKey.getPublicKey();
    auto secp256k1PubKeyData = secp256k1PublicKey.getData();
    assert(secp256k1PubKeyData.size() > 0);
    
    // Тестуємо ChaCha20
    // Test ChaCha20
    // Тестируем ChaCha20
    std::vector<uint8_t> plaintext = {0x01, 0x02, 0x03, 0x04, 0x05};
    std::vector<uint8_t> key(32, 0x12); // 32-byte key
    std::vector<uint8_t> nonce(12, 0x34); // 12-byte nonce
    
    auto ciphertext = ChaCha20::encrypt(plaintext, key, nonce);
    assert(ciphertext.size() == plaintext.size());
    
    auto decrypted = ChaCha20::decrypt(ciphertext, key, nonce);
    assert(decrypted.size() == plaintext.size());
    assert(decrypted == plaintext);
    
    std::cout << "Additional cryptographic algorithms test passed!" << std::endl;
}

int main() {
    std::cout << "Running comprehensive CTON-SDK tests..." << std::endl;
    
    try {
        testCellBasic();
        testCellBuilder();
        testAddress();
        testCrypto();
        testAdditionalCrypto();
        
        std::cout << "\nAll tests passed! CTON-SDK is working correctly." << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Test failed with unknown exception" << std::endl;
        return 1;
    }
}