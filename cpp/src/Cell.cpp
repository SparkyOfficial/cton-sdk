// Cell.cpp - реалізація комірок для TON
// Author: Андрій Будильников (Sparky)
// Реалізація відповідає специфікації TON (TVM):
// - дані зберігаються у бітовому порядку MSB first (big-endian)
// - store-методи ДОПИСУЮТЬ у кінець комірки, а не перезаписують її
// - representation hash: sha256(d1 d2 | padded data | child hashes)

#include "../include/Cell.h"
#include <stdexcept>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <cstddef>

namespace cton {

    namespace {
        uint32_t rotr32(uint32_t x, uint32_t n) {
            return (x >> n) | (x << (32 - n));
        }

        void sha256(const uint8_t* data, size_t len, uint8_t out[32]) {
            static const uint32_t K[64] = {
                0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
                0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
                0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
                0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
                0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
                0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
            };
            uint32_t h0 = 0x6a09e667, h1 = 0xbb67ae85, h2 = 0x3c6ef372, h3 = 0xa54ff53a;
            uint32_t h4 = 0x510e527f, h5 = 0x9b05688c, h6 = 0x1f83d9ab, h7 = 0x5be0cd19;

            uint64_t bitLen = static_cast<uint64_t>(len) * 8;
            size_t padded = len + 1;
            while ((padded % 64) != 56) {
                ++padded;
            }
            padded += 8;
            std::vector<uint8_t> msg(padded, 0);
            if (len > 0) {
                std::memcpy(msg.data(), data, len);
            }
            msg[len] = 0x80;
            for (int i = 0; i < 8; ++i) {
                msg[padded - 1 - i] = static_cast<uint8_t>(bitLen >> (8 * i));
            }

            for (size_t off = 0; off < padded; off += 64) {
                uint32_t w[64];
                for (int i = 0; i < 16; ++i) {
                    w[i] = (static_cast<uint32_t>(msg[off + i * 4]) << 24) |
                           (static_cast<uint32_t>(msg[off + i * 4 + 1]) << 16) |
                           (static_cast<uint32_t>(msg[off + i * 4 + 2]) << 8) |
                           static_cast<uint32_t>(msg[off + i * 4 + 3]);
                }
                for (int i = 16; i < 64; ++i) {
                    uint32_t s0 = rotr32(w[i - 15], 7) ^ rotr32(w[i - 15], 18) ^ (w[i - 15] >> 3);
                    uint32_t s1 = rotr32(w[i - 2], 17) ^ rotr32(w[i - 2], 19) ^ (w[i - 2] >> 10);
                    w[i] = w[i - 16] + s0 + w[i - 7] + s1;
                }
                uint32_t a = h0, b = h1, c = h2, d = h3, e = h4, f = h5, g = h6, h = h7;
                for (int i = 0; i < 64; ++i) {
                    uint32_t S1 = rotr32(e, 6) ^ rotr32(e, 11) ^ rotr32(e, 25);
                    uint32_t ch = (e & f) ^ ((~e) & g);
                    uint32_t t1 = h + S1 + ch + K[i] + w[i];
                    uint32_t S0 = rotr32(a, 2) ^ rotr32(a, 13) ^ rotr32(a, 22);
                    uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
                    uint32_t t2 = S0 + maj;
                    h = g;
                    g = f;
                    f = e;
                    e = d + t1;
                    d = c;
                    c = b;
                    b = a;
                    a = t1 + t2;
                }
                h0 += a; h1 += b; h2 += c; h3 += d; h4 += e; h5 += f; h6 += g; h7 += h;
            }
            uint32_t hs[8] = {h0, h1, h2, h3, h4, h5, h6, h7};
            for (int i = 0; i < 8; ++i) {
                out[i * 4] = static_cast<uint8_t>(hs[i] >> 24);
                out[i * 4 + 1] = static_cast<uint8_t>(hs[i] >> 16);
                out[i * 4 + 2] = static_cast<uint8_t>(hs[i] >> 8);
                out[i * 4 + 3] = static_cast<uint8_t>(hs[i]);
            }
        }
    }
    
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
        // Нормалізуємо: зайві біти в останньому байті мають бути нулями
        data_.resize(expectedByteSize);
        if (bitSize % 8 != 0 && !data_.empty()) {
            uint8_t mask = static_cast<uint8_t>(0xFF << (8 - bitSize % 8));
            data_.back() &= mask;
        }
    }
    
    void Cell::appendBits(size_t count, uint64_t value) {
        if (bitSize_ + count > MAX_BITS) {
            throw std::overflow_error("Not enough space in cell");
        }
        if (count == 0) {
            return;
        }
        
        size_t requiredBytes = (bitSize_ + count + 7) / 8;
        if (data_.size() < requiredBytes) {
            data_.resize(requiredBytes, 0);
        }
        // Очищуємо "хвіст" після поточної позиції (там раніше міг бути padding)
        for (size_t i = requiredBytes - 1; i > (bitSize_ - 1) / 8; --i) {
            data_[i] = 0;
        }
        
        // Запис MSB first (big-endian) — як того вимагає TVM
        size_t pos = bitSize_;
        for (size_t i = count; i-- > 0; ) {
            if ((value >> i) & 1ULL) {
                data_[pos / 8] |= static_cast<uint8_t>(0x80 >> (pos % 8));
            } else {
                data_[pos / 8] &= static_cast<uint8_t>(~(0x80 >> (pos % 8)));
            }
            pos++;
        }
        
        bitSize_ += count;
    }
    
    void Cell::storeUInt(size_t bits, uint64_t value) {
        if (bits > 64) {
            throw std::invalid_argument("storeUInt supports at most 64 bits");
        }
        appendBits(bits, value);
    }
    
    void Cell::storeInt(size_t bits, int64_t value) {
        if (bits == 0 || bits > 64) {
            throw std::invalid_argument("storeInt supports 1..64 bits");
        }
        // two's complement до потрібної ширини
        uint64_t mask = (bits == 64) ? ~0ULL : ((1ULL << bits) - 1);
        appendBits(bits, static_cast<uint64_t>(value) & mask);
    }
    
    void Cell::storeBytes(const std::vector<uint8_t>& bytes) {
        if (bytes.empty()) {
            return;
        }
        if (bitSize_ + bytes.size() * 8 > MAX_BITS) {
            throw std::overflow_error("Not enough space in cell");
        }
        
        if (bitSize_ % 8 == 0) {
            // Вирівняний випадок — просте копіювання
            size_t bytePos = bitSize_ / 8;
            if (data_.size() < bytePos + bytes.size()) {
                data_.resize(bytePos + bytes.size(), 0);
            }
            std::memcpy(data_.data() + bytePos, bytes.data(), bytes.size());
            bitSize_ += bytes.size() * 8;
        } else {
            // Невирівняний випадок — дописуємо побітово
            for (uint8_t b : bytes) {
                appendBits(8, b);
            }
        }
    }
    
    void Cell::storeBit(bool bit) {
        appendBits(1, bit ? 1ULL : 0ULL);
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
    
    uint16_t Cell::depth() const {
        uint16_t depth = 0;
        for (const auto& ref : references_) {
            if (!ref) {
                continue;
            }
            uint16_t child = static_cast<uint16_t>(ref->depth() + 1);
            if (child > depth) {
                depth = child;
            }
        }
        return depth;
    }

    std::shared_ptr<Cell> Cell::clone() const {
        std::vector<std::shared_ptr<Cell>> refs;
        refs.reserve(references_.size());
        for (const auto& ref : references_) {
            if (ref) {
                refs.push_back(ref->clone());
            }
        }
        return std::make_shared<Cell>(data_, bitSize_, refs, isSpecial_);
    }

    std::vector<uint8_t> Cell::representationHash() const {
        std::vector<uint8_t> rep;
        rep.reserve(2 + data_.size() + references_.size() * 34);

        // d1 = refs + 8*exotic + 32*level. Звичайна комірка має level 0.
        uint8_t d1 = static_cast<uint8_t>(references_.size() + (isSpecial_ ? 8 : 0));
        // d2 = floor(b/8) + ceil(b/8)
        uint8_t d2 = static_cast<uint8_t>((bitSize_ / 8) * 2 + ((bitSize_ % 8) != 0 ? 1 : 0));
        rep.push_back(d1);
        rep.push_back(d2);

        size_t dataBytes = (bitSize_ + 7) / 8;
        if (bitSize_ % 8 == 0) {
            rep.insert(rep.end(), data_.begin(), data_.begin() + static_cast<std::ptrdiff_t>(dataBytes));
        } else if (dataBytes > 0) {
            if (dataBytes > 1) {
                rep.insert(rep.end(), data_.begin(), data_.begin() + static_cast<std::ptrdiff_t>(dataBytes - 1));
            }
            size_t r = bitSize_ % 8;
            uint8_t last = static_cast<uint8_t>((data_[dataBytes - 1] & static_cast<uint8_t>(0xFF << (8 - r))) |
                                                 static_cast<uint8_t>(0x80 >> r));
            rep.push_back(last);
        }

        for (const auto& ref : references_) {
            uint16_t childDepth = ref ? ref->depth() : 0;
            rep.push_back(static_cast<uint8_t>(childDepth >> 8));
            rep.push_back(static_cast<uint8_t>(childDepth & 0xFF));
        }
        for (const auto& ref : references_) {
            auto h = ref->representationHash();
            rep.insert(rep.end(), h.begin(), h.end());
        }

        std::vector<uint8_t> hash(32);
        sha256(rep.data(), rep.size(), hash.data());
        return hash;
    }
    
    void Cell::checkCapacity(size_t bitCount) {
        if (bitSize_ + bitCount > MAX_BITS) {
            throw std::overflow_error("Not enough space in cell");
        }
    }
    
    CellBuilder::CellBuilder() : bitOffset_(0) {}
    
    CellBuilder& CellBuilder::storeUInt(size_t bits, uint64_t value) {
        // Перевірка коректності параметрів
        if (bits > 64) {
            throw std::invalid_argument("Bits count cannot exceed 64");
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
        // Очищуємо хвіст після поточної позиції
        for (size_t i = requiredBytes - 1; i > (bitOffset_ - 1) / 8 && bitOffset_ > 0; --i) {
            buffer_[i] = 0;
        }
        
        // Запис бітів у буфер MSB first (big-endian) — вимога TVM
        size_t pos = bitOffset_;
        for (size_t i = bits; i-- > 0; ) {
            if ((value >> i) & 1ULL) {
                buffer_[pos / 8] |= static_cast<uint8_t>(0x80 >> (pos % 8));
            } else {
                buffer_[pos / 8] &= static_cast<uint8_t>(~(0x80 >> (pos % 8)));
            }
            pos++;
        }
        
        bitOffset_ += bits;
        return *this;
    }
    
    CellBuilder& CellBuilder::storeBit(bool bit) {
        return storeUInt(1, bit ? 1ULL : 0ULL);
    }
    
    CellBuilder& CellBuilder::storeInt(size_t bits, int64_t value) {
        if (bits == 0 || bits > 64) {
            throw std::invalid_argument("Bits count must be 1..64");
        }
        // Для від'ємних чисел використовуємо two's complement
        uint64_t mask = (bits == 64) ? ~0ULL : ((1ULL << bits) - 1);
        uint64_t unsignedValue = static_cast<uint64_t>(value) & mask;
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
            // Невирівняний запис: зсув бітів при копіюванні (MSB first)
            uint8_t keep = static_cast<uint8_t>(0xFF << (8 - bitIndex));
            buffer_[byteIndex] &= keep;
            size_t prevLow = 0;
            for (size_t i = 0; i < data.size(); ++i) {
                buffer_[byteIndex + i] |= static_cast<uint8_t>(prevLow | (data[i] >> bitIndex));
                buffer_[byteIndex + i + 1] = static_cast<uint8_t>(data[i] << (8 - bitIndex));
                prevLow = (data[i] << (8 - bitIndex)) & 0xFF;
            }
            // Решта байтів після хвоста має бути нульовою (зайві біти = 0)
            size_t touched = byteIndex + data.size() + 1;
            std::fill(buffer_.begin() + static_cast<long>(touched), buffer_.end(), 0);
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
