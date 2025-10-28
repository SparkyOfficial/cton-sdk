// Cell.cpp - реалізація комірок для TON
// Author: Андрій Будильников (Sparky)

#include "../include/Cell.h"
#include <stdexcept>
#include <cstring>
#include <sstream>

namespace cton {
    
    Cell::Cell() : bitSize_(0), isSpecial_(false) {}
    
    Cell::Cell(const std::vector<uint8_t>& data, 
               size_t bitSize, 
               const std::vector<std::shared_ptr<Cell>>& references,
               bool isSpecial)
        : data_(data), bitSize_(bitSize), references_(references), isSpecial_(isSpecial) {
        // Validate parameters
        if (bitSize > MAX_BITS) {
            throw std::invalid_argument("Bit size exceeds maximum allowed");
        }
        
        if (references.size() > MAX_REFS) {
            throw std::invalid_argument("Number of references exceeds maximum allowed");
        }
        
        // Validate that data size is consistent with bit size
        size_t expectedByteSize = (bitSize + 7) / 8;
        if (data_.size() < expectedByteSize) {
            throw std::invalid_argument("Data size is smaller than expected for given bit size");
        }
    }
    
    void Cell::storeUInt(size_t bits, uint64_t value) {
        CellBuilder builder;
        builder.storeUInt(bits, value);
        auto builtCell = builder.build();
        
        // Copy data from built cell to this cell
        data_ = builtCell->data_;
        bitSize_ = builtCell->bitSize_;
    }
    
    void Cell::storeInt(size_t bits, int64_t value) {
        CellBuilder builder;
        builder.storeInt(bits, value);
        auto builtCell = builder.build();
        
        // Copy data from built cell to this cell
        data_ = builtCell->data_;
        bitSize_ = builtCell->bitSize_;
    }
    
    void Cell::storeBytes(const std::vector<uint8_t>& bytes) {
        CellBuilder builder;
        builder.storeBytes(bytes);
        auto builtCell = builder.build();
        
        // Copy data from built cell to this cell
        data_ = builtCell->data_;
        bitSize_ = builtCell->bitSize_;
    }
    
    void Cell::addReference(std::shared_ptr<Cell> cell) {
        if (!cell) {
            throw std::invalid_argument("Cannot add null reference");
        }
        
        if (references_.size() >= MAX_REFS) {
            throw std::overflow_error("Maximum number of references reached");
        }
        
        references_.push_back(cell);
    }
    
    std::vector<uint8_t> Cell::getData() const {
        return data_;
    }
    
    size_t Cell::getBitSize() const {
        return bitSize_;
    }
    
    std::vector<std::weak_ptr<Cell>> Cell::getReferences() const {
        // Convert shared_ptr to weak_ptr
        std::vector<std::weak_ptr<Cell>> weakRefs;
        weakRefs.reserve(references_.size());
        
        for (const auto& ref : references_) {
            weakRefs.push_back(ref);
        }
        
        return weakRefs;
    }
    
    size_t Cell::getRefsCount() const {
        return references_.size();
    }
    
    bool Cell::isSpecial() const {
        return isSpecial_;
    }
    
    void Cell::checkCapacity(size_t bitCount) {
        if (bitSize_ + bitCount > MAX_BITS) {
            throw std::overflow_error("Not enough space in cell");
        }
    }
    
    CellBuilder::CellBuilder() : bitOffset_(0) {}
    
    CellBuilder& CellBuilder::storeUInt(size_t bits, uint64_t value) {
        // Перевірка коректності параметрів
        if (bits > Cell::MAX_BITS) {
            throw std::invalid_argument("Bits count cannot exceed MAX_BITS");
        }
        
        if (bits == 0) {
            return *this; // Нічого не робимо
        }
        
        // Перевірка чи вистачить місця в комірці
        if (bitOffset_ + bits > Cell::MAX_BITS) {
            throw std::overflow_error("Not enough space in cell for storing bits");
        }
        
        // Розширення буфера при необхідності
        size_t requiredBytes = (bitOffset_ + bits + 7) / 8;
        if (buffer_.size() < requiredBytes) {
            buffer_.resize(requiredBytes, 0);
        }
        
        // Запис бітів у буфер
        size_t byteIndex = bitOffset_ / 8;
        size_t bitIndex = bitOffset_ % 8;
        
        // Якщо біти поміщаються в один байт
        if (bitIndex + bits <= 8) {
            uint8_t mask = ((1ULL << bits) - 1) << (8 - bitIndex - bits);
            buffer_[byteIndex] &= ~mask; // Очищення бітів
            buffer_[byteIndex] |= (value << (8 - bitIndex - bits)) & mask; // Встановлення нових бітів
        } else {
            // Якщо біти розподілені між кількома байтами
            size_t bitsLeft = bits;
            size_t valueShift = 0;
            
            while (bitsLeft > 0) {
                size_t bitsInCurrentByte = std::min(bitsLeft, 8 - bitIndex);
                uint8_t mask = ((1ULL << bitsInCurrentByte) - 1) << (8 - bitIndex - bitsInCurrentByte);
                
                buffer_[byteIndex] &= ~mask; // Очищення бітів
                buffer_[byteIndex] |= ((value >> valueShift) << (8 - bitIndex - bitsInCurrentByte)) & mask;
                
                bitsLeft -= bitsInCurrentByte;
                valueShift += bitsInCurrentByte;
                bitIndex = 0;
                byteIndex++;
                
                // Розширення буфера при необхідності
                if (byteIndex >= buffer_.size() && bitsLeft > 0) {
                    buffer_.resize(byteIndex + 1, 0);
                }
            }
        }
        
        bitOffset_ += bits;
        return *this;
    }
    
    CellBuilder& CellBuilder::storeInt(size_t bits, int64_t value) {
        // Для від'ємних чисел використовуємо two's complement
        uint64_t unsignedValue = static_cast<uint64_t>(value);
        return storeUInt(bits, unsignedValue);
    }
    
    CellBuilder& CellBuilder::storeBytes(const std::vector<uint8_t>& data) {
        if (data.empty()) {
            return *this;
        }
        
        // Перевірка чи вистачить місця в комірці
        size_t bitsNeeded = data.size() * 8;
        if (bitOffset_ + bitsNeeded > Cell::MAX_BITS) {
            throw std::overflow_error("Not enough space in cell for storing bytes");
        }
        
        // Розширення буфера
        size_t requiredBytes = (bitOffset_ + bitsNeeded + 7) / 8;
        if (buffer_.size() < requiredBytes) {
            buffer_.resize(requiredBytes, 0);
        }
        
        // Копіювання байтів
        size_t byteIndex = bitOffset_ / 8;
        size_t bitIndex = bitOffset_ % 8;
        
        if (bitIndex == 0) {
            // Просте копіювання, якщо вирівняно по байтах
            std::memcpy(buffer_.data() + byteIndex, data.data(), data.size());
        } else {
            // Зсув бітів при копіюванні
            for (size_t i = 0; i < data.size(); ++i) {
                uint16_t combined = (buffer_[byteIndex + i] << 8) | data[i];
                buffer_[byteIndex + i] = (combined >> bitIndex) & 0xFF;
                if (byteIndex + i + 1 < buffer_.size()) {
                    buffer_[byteIndex + i + 1] |= (combined << (8 - bitIndex)) & 0xFF;
                }
            }
        }
        
        bitOffset_ += bitsNeeded;
        return *this;
    }
    
    CellBuilder& CellBuilder::storeRef(std::shared_ptr<Cell> cell) {
        if (!cell) {
            throw std::invalid_argument("Cannot store null cell reference");
        }
        
        if (references_.size() >= Cell::MAX_REFS) {
            throw std::overflow_error("Maximum number of references reached");
        }
        
        references_.push_back(cell);
        return *this;
    }
    
    std::shared_ptr<Cell> CellBuilder::build() {
        // Створення комірки через new і shared_ptr
        // This approach avoids the private constructor issue with make_shared
        return std::shared_ptr<Cell>(new Cell(buffer_, bitOffset_, references_, false));
    }
}