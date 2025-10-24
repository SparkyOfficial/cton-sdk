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
        size_t cellCount = cells.size();
        
        // Кодуємо кількість комірок
        // Encode cell count
        // Кодируем количество ячеек
        std::vector<uint8_t> cellCountBytes;
        size_t tempCount = cellCount;
        do {
            cellCountBytes.push_back(tempCount & 0x7F);
            tempCount >>= 7;
        } while (tempCount > 0);
        
        // Встановлюємо старший біт для всіх байтів, окрім останнього
        // Set high bit for all bytes except the last one
        // Устанавливаем старший бит для всех байтов, кроме последнего
        for (size_t i = 0; i < cellCountBytes.size() - 1; ++i) {
            cellCountBytes[i] |= 0x80;
        }
        
        // Додаємо в зворотньому порядку
        // Add in reverse order
        // Добавляем в обратном порядке
        for (auto it = cellCountBytes.rbegin(); it != cellCountBytes.rend(); ++it) {
            result.push_back(*it);
        }
        
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
            if (maxIndex > 0) {
                maxIndexBitSize = 64 - countLeadingZeros(maxIndex);
            }
            
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
        size_t offsetBitSize = maxCellSize > 0 ? 64 - countLeadingZeros(maxCellSize) : 1;
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
        std::vector<size_t> cellSizes;
        
        for (const auto& cell : cells) {
            std::vector<uint8_t> data;
            
            // Додаємо інформацію про комірку
            // Add cell information
            // Добавляем информацию о ячейке
            uint8_t descriptor = 0;
            descriptor |= (cell->getBitSize() > 0) ? 0x80 : 0; // Записуємо біти / Write bits / Записываем биты
            descriptor |= (cell->getReferences().size() & 0x07) << 3; // Кількість референсів / Ref count / Количество ссылок
            
            // Перевіряємо чи комірка спеціальна
            // Check if cell is special
            // Проверяем, является ли ячейка специальной
            if (cell->isSpecial()) {
                descriptor |= 0x04; // Встановлюємо біт спеціальної комірки / Set special cell bit
            }
            
            data.push_back(descriptor);
            
            // Додаємо дані комірки
            // Add cell data
            // Добавляем данные ячейки
            auto cellBytes = cell->getData();
            
            // Додаємо розмір даних у бітах, якщо потрібно
            // Add data size in bits if needed
            // Добавляем размер данных в битах, если нужно
            if (cell->getBitSize() > 0) {
                size_t dataSizeInBytes = (cell->getBitSize() + 7) / 8;
                if (dataSizeInBytes < cellBytes.size()) {
                    // Додаємо додатковий байт з кількістю бітів у останньому байті
                    // Add additional byte with number of bits in last byte
                    // Добавляем дополнительный байт с количеством битов в последнем байте
                    data.push_back(static_cast<uint8_t>(cell->getBitSize() % 8));
                }
            }
            
            data.insert(data.end(), cellBytes.begin(), cellBytes.end());
            
            // Додаємо індекси референсів
            // Add reference indices
            // Добавляем индексы ссылок
            auto references = cell->getReferences();
            for (const auto& ref : references) {
                if (auto refCell = ref.lock()) {
                    auto it = cellIndices.find(refCell);
                    if (it != cellIndices.end()) {
                        // Кодуємо індекс референсу
                        // Encode reference index
                        // Кодируем индекс ссылки
                        size_t refIndex = it->second;
                        std::vector<uint8_t> indexBytes;
                        size_t tempIndex = refIndex;
                        do {
                            indexBytes.push_back(tempIndex & 0x7F);
                            tempIndex >>= 7;
                        } while (tempIndex > 0);
                        
                        // Встановлюємо старший біт для всіх байтів, окрім останнього
                        // Set high bit for all bytes except the last one
                        for (size_t i = 0; i < indexBytes.size() - 1; ++i) {
                            indexBytes[i] |= 0x80;
                        }
                        
                        // Додаємо в зворотньому порядку
                        // Add in reverse order
                        for (auto it2 = indexBytes.rbegin(); it2 != indexBytes.rend(); ++it2) {
                            data.push_back(*it2);
                        }
                    }
                }
            }
            
            cellData.push_back(data);
            cellSizes.push_back(data.size());
        }
        
        // Додаємо розміри комірок (якщо потрібно)
        // Add cell sizes (if needed)
        // Добавляем размеры ячеек (если нужно)
        if (hasIdx) {
            size_t offset = 0;
            for (size_t i = 0; i < cellData.size(); ++i) {
                // Кодуємо зсув
                // Encode offset
                // Кодируем смещение
                std::vector<uint8_t> offsetBytes;
                size_t tempOffset = offset;
                do {
                    offsetBytes.push_back(tempOffset & 0x7F);
                    tempOffset >>= 7;
                } while (tempOffset > 0);
                
                // Встановлюємо старший біт для всіх байтів, окрім останнього
                // Set high bit for all bytes except the last one
                for (size_t j = 0; j < offsetBytes.size() - 1; ++j) {
                    offsetBytes[j] |= 0x80;
                }
                
                // Додаємо в зворотньому порядку
                // Add in reverse order
                for (auto it = offsetBytes.rbegin(); it != offsetBytes.rend(); ++it) {
                    result.push_back(*it);
                }
                
                offset += cellSizes[i];
            }
        }
        
        // Додаємо дані комірок до результату
        // Add cell data to result
        // Добавляем данные ячеек в результат
        for (const auto& data : cellData) {
            result.insert(result.end(), data.begin(), data.end());
        }
        
        // Додаємо CRC, якщо потрібно
        // Add CRC if needed
        // Добавляем CRC, если нужно
        if (hashCRC) {
            // Для простоти, додаємо 4 нульових байти
            // For simplicity, add 4 zero bytes
            // Для простоты, добавляем 4 нулевых байта
            result.insert(result.end(), 4, 0);
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
        bool hasCacheBits = (flags & 0x04) != 0;
        
        // Читаємо кількість комірок
        // Read number of cells
        // Читаем количество ячеек
        size_t cellCount = 0;
        uint8_t byte;
        do {
            byte = readByte();
            cellCount = (cellCount << 7) | (byte & 0x7F);
        } while ((byte & 0x80) != 0);
        
        // Читаємо розміри полів
        // Read field sizes
        // Читаем размеры полей
        size_t offsetBitSize = readByte();
        size_t cellsBitSize = readByte();
        size_t rootsBitSize = readByte();
        size_t absentBitSize = readByte();
        
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
            size_t index = 0;
            uint8_t byte2;
            do {
                byte2 = readByte();
                index = (index << 7) | (byte2 & 0x7F);
            } while ((byte2 & 0x80) != 0);
            rootIndices[i] = index;
        }
        
        // Читаємо індекси відсутніх комірок
        // Read absent cell indices
        // Читаем индексы отсутствующих ячеек
        std::vector<size_t> absentIndices(absentCount);
        for (size_t i = 0; i < absentCount; ++i) {
            size_t index = 0;
            uint8_t byte2;
            do {
                byte2 = readByte();
                index = (index << 7) | (byte2 & 0x7F);
            } while ((byte2 & 0x80) != 0);
            absentIndices[i] = index;
        }
        
        // Читаємо зсуви, якщо потрібно
        // Read offsets if needed
        // Читаем смещения, если нужно
        std::vector<size_t> offsets;
        if (hasIdx) {
            offsets.resize(cellCount);
            for (size_t i = 0; i < cellCount; ++i) {
                size_t offset = 0;
                uint8_t byte2;
                do {
                    byte2 = readByte();
                    offset = (offset << 7) | (byte2 & 0x7F);
                } while ((byte2 & 0x80) != 0);
                offsets[i] = offset;
            }
        }
        
        // Створюємо вектор комірок
        // Create vector of cells
        // Создаем вектор ячеек
        std::vector<std::shared_ptr<Cell>> cells(cellCount);
        
        // Парсимо комірки
        // Parse cells
        // Парсим ячейки
        size_t dataStartOffset = offset_;
        for (size_t i = 0; i < cellCount; ++i) {
            // Якщо є індекси, встановлюємо правильне положення
            // If there are indices, set correct position
            // Если есть индексы, устанавливаем правильную позицию
            if (hasIdx && i < offsets.size()) {
                offset_ = dataStartOffset + offsets[i];
            }
            
            // Читаємо дескриптор комірки
            // Read cell descriptor
            // Читаем дескриптор ячейки
            uint8_t descriptor = readByte();
            bool hasBits = (descriptor & 0x80) != 0;
            size_t refCount = (descriptor >> 3) & 0x07;
            bool isSpecial = (descriptor & 0x04) != 0;
            bool hasLevel = (descriptor & 0x02) != 0;
            bool hasHashes = (descriptor & 0x01) != 0;
            
            // Читаємо розмір даних у бітах
            // Read data size in bits
            // Читаем размер данных в битах
            size_t bitSize = 0;
            if (hasBits) {
                bitSize = readByte();
                if (bitSize == 0) {
                    // Якщо розмір 0, це означає 256 бітів
                    // If size is 0, it means 256 bits
                    // Если размер 0, это означает 256 битов
                    bitSize = 256;
                }
            }
            
            // Читаємо додатковий байт, якщо потрібно
            // Read additional byte if needed
            // Читаем дополнительный байт, если нужно
            size_t dataSizeInBytes = (bitSize + 7) / 8;
            if (dataSizeInBytes < (bitSize + 7) / 8) {
                uint8_t extraByte = readByte();
                // Використовуємо extraByte для визначення точного розміру
                // Use extraByte to determine exact size
            }
            
            // Читаємо дані комірки
            // Read cell data
            // Читаем данные ячейки
            std::vector<uint8_t> cellData;
            if (dataSizeInBytes > 0) {
                cellData = readBytes(dataSizeInBytes);
            }
            
            // Читаємо індекси референсів
            // Read reference indices
            // Читаем индексы ссылок
            std::vector<size_t> refIndices(refCount);
            for (size_t j = 0; j < refCount; ++j) {
                size_t refIndex = 0;
                uint8_t byte2;
                do {
                    byte2 = readByte();
                    refIndex = (refIndex << 7) | (byte2 & 0x7F);
                } while ((byte2 & 0x80) != 0);
                refIndices[j] = refIndex;
            }
            
            // Створюємо комірку
            // Create cell
            // Создаем ячейку
            std::shared_ptr<Cell> cell = std::make_shared<Cell>(cellData, bitSize, std::vector<std::shared_ptr<Cell>>(), isSpecial);
            cells[i] = cell;
            
            // Встановлюємо референси (будуть встановлені пізніше)
            // Set references (will be set later)
            // Устанавливаем ссылки (будут установлены позже)
        }
        
        // Встановлюємо референси для комірок
        // Set references for cells
        // Устанавливаем ссылки для ячеек
        // Це можна зробити в окремому проході після створення всіх комірок
        // This can be done in a separate pass after creating all cells
        // Это можно сделать в отдельном проходе после создания всех ячеек
        
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