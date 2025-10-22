// Boc.cpp - робота з Bag of Cells (BOC)
// Author: Андрій Будильников (Sparky)
// Реалізація Bag of Cells - основного формату серіалізації даних в TON
// Implementation of Bag of Cells - the main data serialization format in TON
// Реализация Bag of Cells - основного формата сериализации данных в TON

#include "../include/Boc.h"
#include <stdexcept>
#include <algorithm>
#include <cstring>

namespace cton {
    
    Boc::Boc() : root_(nullptr) {}
    
    Boc::Boc(std::shared_ptr<Cell> root) : root_(root) {}
    
    std::vector<uint8_t> Boc::serialize(bool hasIdx, bool hashCRC) const {
        // В реальній реалізації тут має бути серіалізація BOC в бінарне представлення
        // In real implementation, BOC serialization to binary representation should be here
        // В реальной реализации здесь должна быть сериализация BOC в бинарное представление
        
        // Поки що просто повертаємо порожній вектор (це неправильно!)
        // For now just return empty vector (this is wrong!)
        // Пока что просто возвращаем пустой вектор (это неправильно!)
        
        return std::vector<uint8_t>();
    }
    
    Boc Boc::deserialize(const std::vector<uint8_t>& data) {
        // В реальній реалізації тут має бути десеріалізація BOC з бінарного представлення
        // In real implementation, BOC deserialization from binary representation should be here
        // В реальной реализации здесь должна быть десериализация BOC из бинарного представления
        
        // Поки що просто повертаємо порожній BOC (це неправильно!)
        // For now just return empty BOC (this is wrong!)
        // Пока что просто возвращаем пустой BOC (это неправильно!)
        
        return Boc();
    }
    
    std::shared_ptr<Cell> Boc::getRoot() const {
        return root_;
    }
    
    void Boc::setRoot(std::shared_ptr<Cell> root) {
        root_ = root;
    }
    
    void Boc::collectCells(std::shared_ptr<Cell> cell, 
                         std::vector<std::shared_ptr<Cell>>& cells,
                         std::vector<std::shared_ptr<Cell>>& visited) const {
        // Перевірка чи комірка вже відвідана
        // Check if cell is already visited
        // Проверка, посещена ли ячейка уже
        
        for (const auto& visitedCell : visited) {
            if (visitedCell == cell) {
                return; // Комірка вже відвідана / Cell already visited / Ячейка уже посещена
            }
        }
        
        // Додати комірку до відвіданих
        // Add cell to visited
        // Добавить ячейку в посещенные
        visited.push_back(cell);
        
        // Додати комірку до колекції
        // Add cell to collection
        // Добавить ячейку в коллекцию
        cells.push_back(cell);
        
        // Рекурсивно обробити референси
        // Recursively process references
        // Рекурсивно обработать ссылки
        auto references = cell->getReferences();
        for (const auto& ref : references) {
            if (auto refCell = ref.lock()) {
                collectCells(refCell, cells, visited);
            }
        }
    }
    
    BocParser::BocParser(const std::vector<uint8_t>& data) : data_(data), offset_(0) {}
    
    Boc BocParser::parse() {
        // В реальній реалізації тут має бути парсинг BOC
        // In real implementation, BOC parsing should be here
        // В реальной реализации здесь должен быть парсинг BOC
        
        // Поки що просто повертаємо порожній BOC (це неправильно!)
        // For now just return empty BOC (this is wrong!)
        // Пока что просто возвращаем пустой BOC (это неправильно!)
        
        return Boc();
    }
    
    uint8_t BocParser::readByte() {
        if (offset_ >= data_.size()) {
            throw std::out_of_range("Not enough data to read byte");
        }
        return data_[offset_++];
    }
    
    uint32_t BocParser::readUint32() {
        if (offset_ + 4 > data_.size()) {
            throw std::out_of_range("Not enough data to read uint32");
        }
        
        uint32_t value = 0;
        value |= static_cast<uint32_t>(data_[offset_++]) << 24;
        value |= static_cast<uint32_t>(data_[offset_++]) << 16;
        value |= static_cast<uint32_t>(data_[offset_++]) << 8;
        value |= static_cast<uint32_t>(data_[offset_++]);
        
        return value;
    }
    
    std::vector<uint8_t> BocParser::readBytes(size_t size) {
        if (offset_ + size > data_.size()) {
            throw std::out_of_range("Not enough data to read bytes");
        }
        
        std::vector<uint8_t> result(data_.begin() + offset_, data_.begin() + offset_ + size);
        offset_ += size;
        return result;
    }
    
    BocBuilder::BocBuilder(std::shared_ptr<Cell> root) : root_(root) {}
    
    std::vector<uint8_t> BocBuilder::build(bool hasIdx, bool hashCRC) {
        // В реальній реалізації тут має бути побудова BOC
        // In real implementation, BOC building should be here
        // В реальной реализации здесь должна быть постройка BOC
        
        // Поки що просто повертаємо порожній вектор (це неправильно!)
        // For now just return empty vector (this is wrong!)
        // Пока что просто возвращаем пустой вектор (это неправильно!)
        
        return std::vector<uint8_t>();
    }
}