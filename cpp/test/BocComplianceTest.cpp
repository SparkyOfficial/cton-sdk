// BocComplianceTest.cpp - тестування відповідності формату BOC
// Author: Андрій Будильников (Sparky)
// BOC format compliance testing
// Тестирование соответствия формата BOC

#include "../include/Boc.h"
#include "../include/Cell.h"
#include <iostream>
#include <cassert>
#include <vector>

using namespace cton;

int main() {
    std::cout << "Testing BOC format compliance..." << std::endl;
    
    try {
        // Створюємо просту комірку
        // Create a simple cell
        // Создаем простую ячейку
        std::cout << "1. Creating a simple cell..." << std::endl;
        CellBuilder builder;
        builder.storeUInt(32, 0x12345678);
        builder.storeBytes({0x01, 0x02, 0x03, 0x04});
        auto cell = builder.build();
        
        // Створюємо BOC
        // Create BOC
        // Создаем BOC
        std::cout << "2. Creating BOC..." << std::endl;
        Boc boc(cell);
        
        // Серіалізуємо з різними параметрами
        // Serialize with different parameters
        // Сериализуем с разными параметрами
        std::cout << "3. Serializing BOC with index and CRC..." << std::endl;
        auto serializedWithIdxCRC = boc.serialize(true, true);
        std::cout << "   Serialized size with index and CRC: " << serializedWithIdxCRC.size() << " bytes" << std::endl;
        
        std::cout << "4. Serializing BOC without index and CRC..." << std::endl;
        auto serializedWithoutIdxCRC = boc.serialize(false, false);
        std::cout << "   Serialized size without index and CRC: " << serializedWithoutIdxCRC.size() << " bytes" << std::endl;
        
        // Десеріалізуємо
        // Deserialize
        // Десериализуем
        std::cout << "5. Deserializing BOC..." << std::endl;
        auto deserialized = Boc::deserialize(serializedWithIdxCRC);
        auto root = deserialized.getRoot();
        assert(root != nullptr);
        std::cout << "   Deserialization successful" << std::endl;
        
        // Перевіряємо розмір даних
        // Check data size
        // Проверяем размер данных
        std::cout << "6. Checking data consistency..." << std::endl;
        assert(root->getBitSize() == cell->getBitSize());
        std::cout << "   Data size consistency: OK" << std::endl;
        
        // Створюємо складнішу структуру з референсами
        // Create a more complex structure with references
        // Создаем более сложную структуру с ссылками
        std::cout << "7. Creating complex cell structure..." << std::endl;
        CellBuilder builder1;
        builder1.storeUInt(32, 0x11111111);
        
        CellBuilder builder2;
        builder2.storeUInt(32, 0x22222222);
        
        CellBuilder builder3;
        builder3.storeUInt(32, 0x33333333);
        builder3.storeRef(builder1.build());
        builder3.storeRef(builder2.build());
        
        auto complexCell = builder3.build();
        assert(complexCell->getRefsCount() == 2);
        std::cout << "   Complex structure created with " << complexCell->getRefsCount() << " references" << std::endl;
        
        // Створюємо BOC зі складною структурою
        // Create BOC with complex structure
        // Создаем BOC со сложной структурой
        std::cout << "8. Creating BOC with complex structure..." << std::endl;
        Boc complexBoc(complexCell);
        auto complexSerialized = complexBoc.serialize(true, true);
        std::cout << "   Complex BOC size: " << complexSerialized.size() << " bytes" << std::endl;
        
        // Десеріалізуємо складну структуру
        // Deserialize complex structure
        // Десериализуем сложную структуру
        std::cout << "9. Deserializing complex BOC..." << std::endl;
        auto complexDeserialized = Boc::deserialize(complexSerialized);
        auto complexRoot = complexDeserialized.getRoot();
        assert(complexRoot != nullptr);
        assert(complexRoot->getRefsCount() == 2);
        std::cout << "   Complex deserialization successful with " << complexRoot->getRefsCount() << " references" << std::endl;
        
        std::cout << "\nBOC format compliance test completed successfully!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in BOC compliance test: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error in BOC compliance test" << std::endl;
        return 1;
    }
}