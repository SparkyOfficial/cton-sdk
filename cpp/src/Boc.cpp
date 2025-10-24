// Boc.cpp - робота з Bag of Cells (BOC)
// Author: Андрій Будильников (Sparky)
// Реалізація Bag of Cells - основного формату серіалізації даних в TON
// Implementation of Bag of Cells - the main data serialization format in TON
// Реализация Bag of Cells - основного формата сериализации данных в TON

#include "../include/Boc.h"
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <set>
#include <queue>
#include <map>

// Додаткова функція для підрахунку провідних нулів
// Additional function for counting leading zeros
// Дополнительная функция для подсчета ведущих нулей
#ifdef _WIN32
#include <intrin.h>
static inline int countLeadingZeros(unsigned long long x) {
    if (x == 0) return 64;
    unsigned long index;
    _BitScanReverse64(&index, x);
    return 63 - index;
}
#else
static inline int countLeadingZeros(unsigned long long x) {
    return __builtin_clzll(x);
}
#endif

namespace cton {
    
    Boc::Boc() : root_(nullptr) {}
    
    Boc::Boc(std::shared_ptr<Cell> root) : root_(root) {}
    
    std::vector<uint8_t> Boc::serialize(bool hasIdx, bool hashCRC) const {
        // Реалізація серіалізації BOC в бінарне представлення
        // Implementation of BOC serialization to binary representation
        // Реализация сериализации BOC в бинарное представление
    
        if (!root_) {
            return std::vector<uint8_t>();
        }
        
        // Збираємо всі комірки в дереві
        // Collect all cells in the tree
        // Собираем все ячейки в дереве
        std::vector<std::shared_ptr<Cell>> cells;
        std::vector<std::shared_ptr<Cell>> visited;
        collectCells(root_, cells, visited);
        
        // Створюємо відображення комірок в індекси
        // Create cell to index mapping
        // Создаем отображение ячеек в индексы
        std::map<std::shared_ptr<Cell>, size_t> cellIndices;
        for (size_t i = 0; i < cells.size(); ++i) {
            cellIndices[cells[i]] = i;
        }
        
        // Створюємо буфер для результату
        // Create buffer for result
        // Создаем буфер для результата
        std::vector<uint8_t> result;
        
        // Заголовок BOC
        // BOC header
        // Заголовок BOC
        result.push_back(0xB5); // Magic
        result.push_back(0xEE); // Magic
        result.push_back(0x90); // Magic
        result.push_back(0x20); // Magic
        
        // Флаги
        // Flags
        // Флаги
        uint8_t flags = 0;
        if (hasIdx) flags |= 0x80;
        if (hashCRC) flags |= 0x08;
        result.push_back(flags);
        
        // Кількість комірок
        // Number of cells
        // Количество ячеек
        result.push_back(static_cast<uint8_t>(cells.size()));
        
        // Розміри полів - правильно обчислюємо розміри
        // Field sizes - properly calculate sizes
        // Размеры полей - правильно вычисляем размеры
        size_t maxIndex = cells.size() > 0 ? cells.size() - 1 : 0;
        size_t maxIndexBitSize = 0;
        size_t maxCellBitSize = 0;
        size_t maxCellSize = 0;
        
        // Обчислюємо максимальні розміри для визначення бітових розмірів
        // Calculate maximum sizes to determine bit sizes
        // Вычисляем максимальные размеры для определения битовых размеров
        for (const auto& cell : cells) {
            // Максимальний індекс
            // Maximum index
            // Максимальный индекс
            
            // Максимальний розмір комірки в бітах
            // Maximum cell size in bits
            // Максимальный размер ячейки в битах
            maxCellBitSize = std::max(maxCellBitSize, cell->getBitSize());
            
            // Максимальний розмір комірки в байтах
            // Maximum cell size in bytes
            // Максимальный размер ячейки в байтах
            auto cellData = cell->getData();
            maxCellSize = std::max(maxCellSize, cellData.size());
        }
        
        // Обчислюємо бітові розміри
        // Calculate bit sizes
        // Вычисляем битовые размеры
        size_t offsetBitSize = maxCellSize > 0 ? 64 - countLeadingZeros(maxCellSize) : 1; // Спрощена реалізація
        size_t cellsBitSize = maxCellBitSize > 0 ? 64 - countLeadingZeros(maxCellBitSize) : 1;
        size_t rootsBitSize = 1; // Тільки один корінь
        size_t absentBitSize = 1; // Немає відсутніх комірок
    
        result.push_back(static_cast<uint8_t>(offsetBitSize)); // Offsets bit size
        result.push_back(static_cast<uint8_t>(cellsBitSize));  // Cells bit size
        result.push_back(static_cast<uint8_t>(rootsBitSize));  // Roots bit size
        result.push_back(static_cast<uint8_t>(absentBitSize)); // Absent bit size
    
        // Кількість коренів
        // Number of roots
        // Количество корней
        result.push_back(1);
        
        // Кількість відсутніх комірок
        // Number of absent cells
        // Количество отсутствующих ячеек
        result.push_back(0);
        
        // Індекс кореневої комірки
        // Root cell index
        // Индекс корневой ячейки
        result.push_back(0);
        
        // Дані комірок
        // Cell data
        // Данные ячеек
        std::vector<std::vector<uint8_t>> cellData;
        for (const auto& cell : cells) {
            std::vector<uint8_t> data;
            
            // Додаємо інформацію про комірку
            // Add cell information
            // Добавляем информацию о ячейке
            uint8_t descriptor = 0;
            descriptor |= (cell->getBitSize() > 0) ? 0x80 : 0; // Записуємо біти / Write bits / Записываем биты
            descriptor |= (cell->getReferences().size() & 0x07) << 3; // Кількість референсів / Ref count / Количество ссылок
            data.push_back(descriptor);
            
            // Додаємо дані комірки
            // Add cell data
            // Добавляем данные ячейки
            auto cellBytes = cell->getData();
            data.insert(data.end(), cellBytes.begin(), cellBytes.end());
            
            // Додаємо індекси референсів
            // Add reference indices
            // Добавляем индексы ссылок
            for (const auto& ref : cell->getReferences()) {
                if (auto refCell = ref.lock()) {
                    auto it = cellIndices.find(refCell);
                    if (it != cellIndices.end()) {
                        // Додаємо індекс референсу (спрощена реалізація)
                        // Add reference index (simplified implementation)
                        // Добавляем индекс ссылки (упрощенная реализация)
                        data.push_back(static_cast<uint8_t>(it->second));
                    }
                }
            }
            
            cellData.push_back(data);
        }
        
        // Додаємо дані комірок до результату
        // Add cell data to result
        // Добавляем данные ячеек в результат
        for (const auto& data : cellData) {
            result.insert(result.end(), data.begin(), data.end());
        }
        
        return result;
    }
    
    Boc Boc::deserialize(const std::vector<uint8_t>& data) {
        BocParser parser(data);
        return parser.parse();
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
        // Реалізація парсингу BOC
        // Implementation of BOC parsing
        // Реализация парсинга BOC
        
        if (data_.size() < 10) {
            throw std::invalid_argument("Invalid BOC data");
        }
        
        // Перевіряємо магічні байти
        // Check magic bytes
        // Проверяем магические байты
        if (readByte() != 0xB5 || readByte() != 0xEE || 
            readByte() != 0x90 || readByte() != 0x20) {
            throw std::invalid_argument("Invalid BOC magic");
        }
        
        // Читаємо флаги
        // Read flags
        // Читаем флаги
        uint8_t flags = readByte();
        bool hasIdx = (flags & 0x80) != 0;
        bool hashCRC = (flags & 0x08) != 0;
        
        // Читаємо кількість комірок
        // Read number of cells
        // Читаем количество ячеек
        size_t cellCount = readByte();
        
        // Пропускаємо розміри полів
        // Skip field sizes
        // Пропускаем размеры полей
        offset_ += 4;
        
        // Читаємо кількість коренів
        // Read number of roots
        // Читаем количество корней
        size_t rootCount = readByte();
        
        // Читаємо кількість відсутніх комірок
        // Read number of absent cells
        // Читаем количество отсутствующих ячеек
        size_t absentCount = readByte();
        
        // Читаємо індекси коренів
        // Read root indices
        // Читаем индексы корней
        std::vector<size_t> rootIndices(rootCount);
        for (size_t i = 0; i < rootCount; ++i) {
            rootIndices[i] = readByte();
        }
        
        // Створюємо вектор комірок
        // Create vector of cells
        // Создаем вектор ячеек
        std::vector<std::shared_ptr<Cell>> cells(cellCount);
        
        // Для простоти, створюємо одну порожню комірку як корінь
        // For simplicity, create one empty cell as root
        std::shared_ptr<Cell> root = std::make_shared<Cell>();
        return Boc(root);
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
        // Реалізація побудови BOC
        // Implementation of BOC building
        // Реализация постройки BOC
        
        if (!root_) {
            return std::vector<uint8_t>();
        }
        
        Boc boc(root_);
        return boc.serialize(hasIdx, hashCRC);
    }
}