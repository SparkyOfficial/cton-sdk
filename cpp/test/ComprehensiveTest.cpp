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
    assert(cell.storeUInt(8, 0xFF));
    assert(cell.storeInt(8, -1));
    assert(cell.storeBytes({0x01, 0x02, 0x03}));
    
    // Перевіряємо розмір
    // Check size
    // Проверяем размер
    assert(cell.getBitSize() > 0);
    
    // Перевіряємо кількість референсів
    // Check reference count
    // Проверяем количество ссылок
    assert(cell.getReferencesCount() == 0);
    
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

void testBoc() {
    std::cout << "Testing BOC functionality..." << std::endl;
    
    // Створюємо комірку
    // Create cell
    // Создаем ячейку
    CellBuilder builder;
    builder.storeUInt(32, 0x12345678)
           .storeBytes({0x01, 0x02, 0x03, 0x04});
    auto cell = builder.build();
    
    // Створюємо BOC
    // Create BOC
    // Создаем BOC
    Boc boc(cell);
    
    // Серіалізуємо
    // Serialize
    // Сериализуем
    auto serialized = boc.serialize(true, true);
    assert(!serialized.empty());
    
    // Десеріалізуємо
    // Deserialize
    // Десериализуем
    auto deserialized = Boc::deserialize(serialized);
    auto root = deserialized.getRoot();
    assert(root != nullptr);
    
    std::cout << "BOC functionality test passed!" << std::endl;
}

int main() {
    std::cout << "Running comprehensive CTON-SDK tests..." << std::endl;
    
    try {
        testCellBasic();
        testCellBuilder();
        testAddress();
        testCrypto();
        testBoc();
        
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