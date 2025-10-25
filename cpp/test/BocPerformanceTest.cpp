// BocPerformanceTest.cpp - тестування продуктивності BOC
// Author: Андрій Будильников (Sparky)
// BOC performance testing
// Тестирование производительности BOC

#include "../include/Boc.h"
#include "../include/Cell.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <memory>

using namespace cton;

int main() {
    std::cout << "Testing BOC performance..." << std::endl;
    
    try {
        // Створюємо складну структуру з багатьма комірками
        // Create a complex structure with many cells
        // Создаем сложную структуру с множеством ячеек
        std::cout << "1. Creating complex cell structure..." << std::endl;
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Створюємо 1000 комірок у ланцюжку
        // Create 1000 cells in a chain
        // Создаем 1000 ячеек в цепочке
        std::shared_ptr<Cell> root = std::make_shared<Cell>();
        root->storeUInt(32, 0x12345678);
        
        std::shared_ptr<Cell> current = root;
        for (int i = 0; i < 1000; ++i) {
            auto newCell = std::make_shared<Cell>();
            newCell->storeUInt(32, i);
            newCell->storeBytes({static_cast<uint8_t>(i), static_cast<uint8_t>(i >> 8)});
            current->addReference(newCell);
            current = newCell;
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "   Structure creation time: " << duration.count() << " microseconds" << std::endl;
        
        // Створюємо BOC
        // Create BOC
        // Создаем BOC
        std::cout << "2. Creating BOC..." << std::endl;
        start = std::chrono::high_resolution_clock::now();
        
        Boc boc(root);
        
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "   BOC creation time: " << duration.count() << " microseconds" << std::endl;
        
        // Серіалізуємо BOC
        // Serialize BOC
        // Сериализуем BOC
        std::cout << "3. Serializing BOC..." << std::endl;
        start = std::chrono::high_resolution_clock::now();
        
        auto serialized = boc.serialize(true, true);
        
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "   Serialization time: " << duration.count() << " microseconds" << std::endl;
        std::cout << "   Serialized size: " << serialized.size() << " bytes" << std::endl;
        
        // Десеріалізуємо BOC
        // Deserialize BOC
        // Десериализуем BOC
        std::cout << "4. Deserializing BOC..." << std::endl;
        start = std::chrono::high_resolution_clock::now();
        
        auto deserialized = Boc::deserialize(serialized);
        
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "   Deserialization time: " << duration.count() << " microseconds" << std::endl;
        
        // Перевіряємо результат
        // Check result
        // Проверяем результат
        auto rootCell = deserialized.getRoot();
        if (rootCell) {
            std::cout << "   Deserialization successful" << std::endl;
        } else {
            std::cout << "   Deserialization failed" << std::endl;
            return 1;
        }
        
        // Тест з багатьма референсами
        // Test with many references
        // Тест с множеством ссылок
        std::cout << "5. Testing with multiple references..." << std::endl;
        start = std::chrono::high_resolution_clock::now();
        
        CellBuilder builder;
        builder.storeUInt(32, 0xABCDEF00);
        
        // Додаємо 100 референсів
        // Add 100 references
        // Добавляем 100 ссылок
        for (int i = 0; i < 100; ++i) {
            CellBuilder refBuilder;
            refBuilder.storeUInt(32, i);
            builder.storeRef(refBuilder.build());
        }
        
        auto complexCell = builder.build();
        Boc complexBoc(complexCell);
        auto complexSerialized = complexBoc.serialize(true, true);
        auto complexDeserialized = Boc::deserialize(complexSerialized);
        
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "   Complex structure test time: " << duration.count() << " microseconds" << std::endl;
        std::cout << "   Complex serialized size: " << complexSerialized.size() << " bytes" << std::endl;
        
        std::cout << "\nBOC performance test completed successfully!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in BOC performance test: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error in BOC performance test" << std::endl;
        return 1;
    }
}