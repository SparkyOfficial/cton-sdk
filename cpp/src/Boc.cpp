// Boc.cpp - робота з Bag of Cells (BOC)
// Author: Андрій Будильников (Sparky)
// Реалізація Bag of Cells - основного формату серіалізації даних в TON
// Implementation of Bag of Cells - the main data serialization format in TON
// Реализация Bag of Cells - основного формата сериализации данных в TON
//
// Формат — Generic BoC з ton-blockchain (serialized_boc#b5ee9c72):
// - magic: 0x68FF65F3 (без CRC) або 0xB5EE9C72 (з CRC32-C)
// - прапори: has_idx:1 has_crc32c:1 has_cache_bits:1 flags:2 size:3, далі окремий байт off_bytes
// - індекс референсу завжди більший за індекс комірки, корінь має індекс 0
// - d1 = refs + 8*exotic + 32*level (+16, якщо в чанку збережені хеші)
// - d2 = floor(bits/8) + ceil(bits/8); неповний байт має completion bit
// - контрольна сума — CRC32-C (Castagnoli), little-endian

#include "../include/Boc.h"
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <set>
#include <queue>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <cstddef>

namespace cton {
    
    Boc::Boc() : root_(nullptr) {}
    
    Boc::Boc(std::shared_ptr<Cell> root) : root_(root) {}
    
    namespace {
        // Запис цілого числа big-endian у вказаній кількості байтів
        void appendBe(std::vector<uint8_t>& out, uint64_t value, size_t bytes) {
            for (size_t i = bytes; i-- > 0; ) {
                out.push_back(static_cast<uint8_t>((value >> (i * 8)) & 0xFF));
            }
        }
        
        // Кількість байтів, потрібних для запису значення (1..7)
        size_t byteLenFor(uint64_t value) {
            size_t n = 1;
            while (n < 8 && (value >> (n * 8)) != 0) {
                n++;
            }
            return n;
        }
        
        // Зібрання комірок у топологічному порядку: референси попереду, корінь останнім
        void collectTopo(const std::shared_ptr<Cell>& cell,
                        std::vector<std::shared_ptr<Cell>>& cells,
                        std::unordered_set<const Cell*>& visited) {
            if (visited.count(cell.get()) != 0) {
                return;
            }
            visited.insert(cell.get());
            for (const auto& ref : cell->getReferences()) {
                if (auto refCell = ref.lock()) {
                    collectTopo(refCell, cells, visited);
                }
            }
            cells.push_back(cell);
        }
    }
    
    std::vector<uint8_t> Boc::serialize(bool hasIdx, bool hashCRC) const {
        // hasIdx збережено для сумісності API; секція індексу завжди записується,
        // оскільки основні бібліотеки TON очікують именно Generic BoC з індексом.
        (void)hasIdx;
    
        if (!root_) {
            return std::vector<uint8_t>();
        }
        
        // Post-order (діти попереду), потім розворот: індекс референсу завжди більший.
        // Корінь отримує індекс 0, як у стандартному BagOfCells.
        std::vector<std::shared_ptr<Cell>> cells;
        std::unordered_set<const Cell*> visited;
        collectTopo(root_, cells, visited);
        std::reverse(cells.begin(), cells.end());
        
        const size_t n = cells.size();
        const bool withCrc = hashCRC;
        const bool withIndex = hasIdx;
        const size_t sz = byteLenFor(n == 0 ? 1 : n);
        
        // d1 = refs + 8*exotic (level 0, без біта with_hashes)
        auto d1Of = [](const std::shared_ptr<Cell>& cell) -> uint8_t {
            return static_cast<uint8_t>(cell->getRefsCount() + (cell->isSpecial() ? 8 : 0));
        };
        // d2 = floor(bits/8) + ceil(bits/8)
        auto d2Of = [](size_t bits) -> uint8_t {
            return static_cast<uint8_t>((bits / 8) * 2 + ((bits % 8) != 0 ? 1 : 0));
        };
        
        // Попередній розрахунок розмірів чанків комірок
        std::vector<size_t> chunkSizes(n);
        uint64_t totalCells = 0;
        for (size_t i = 0; i < n; ++i) {
            size_t bits = cells[i]->getBitSize();
            size_t refs = cells[i]->getRefsCount();
            chunkSizes[i] = 2 + (bits + 7) / 8 + refs * sz;
            totalCells += chunkSizes[i];
        }
        size_t offSz = byteLenFor(totalCells == 0 ? 1 : totalCells);
        
        uint64_t serialSize = 2 + 3 * sz + offSz + sz +
                              (withIndex ? static_cast<uint64_t>(n) * offSz : 0) + totalCells;
        
        std::vector<uint8_t> result;
        result.reserve(serialSize + (withCrc ? 10 : 6));
        
        // Magic: Generic BoC з CRC32-C або без
        if (withCrc) {
            result.push_back(0xB5); result.push_back(0xEE);
            result.push_back(0x9C); result.push_back(0x72);
        } else {
            result.push_back(0x68); result.push_back(0xFF);
            result.push_back(0x65); result.push_back(0xF3);
        }
        
        // serialized_boc: has_idx has_crc32c has_cache_bits flags:2 size:3 , потім off_bytes:uint8
        uint8_t flagsByte = static_cast<uint8_t>(sz & 0x07);
        if (withIndex) {
            flagsByte |= 0x80;
        }
        if (withCrc) {
            flagsByte |= 0x40;
        }
        result.push_back(flagsByte);
        result.push_back(static_cast<uint8_t>(offSz));
        
        appendBe(result, n, sz);                 // cells
        appendBe(result, 1, sz);                 // roots
        appendBe(result, 0, sz);                 // absent
        appendBe(result, totalCells, offSz);     // tot_cells_size
        appendBe(result, 0, sz);                    // root_index (корінь — перша комірка)
        
        if (withIndex) {
            uint64_t running = 0;
            for (size_t i = 0; i < n; ++i) {
                running += chunkSizes[i];
                appendBe(result, running, offSz);
            }
        }
        
        // Дані комірок: d1, d2, bytes (+ completion bit), індекси референсів
        std::unordered_map<const Cell*, size_t> cellIndices;
        cellIndices.reserve(n);
        for (size_t i = 0; i < n; ++i) {
            cellIndices[cells[i].get()] = i;
        }
        
        for (size_t i = 0; i < n; ++i) {
            const auto& cell = cells[i];
            const auto& data = cell->getData();
            size_t bits = cell->getBitSize();
            
            result.push_back(d1Of(cell));
            result.push_back(d2Of(bits));
            
            size_t dataBytes = (bits + 7) / 8;
            if (bits % 8 == 0) {
                result.insert(result.end(), data.begin(), data.begin() + static_cast<long>(dataBytes));
            } else {
                // дописуємо completion bit (1) і нулі до межі байта
                size_t r = bits % 8;
                for (size_t j = 0; j + 1 < dataBytes; ++j) {
                    result.push_back(data[j]);
                }
                uint8_t lastByte = static_cast<uint8_t>((data[dataBytes - 1] & (0xFF << (8 - r))) | (0x80 >> r));
                result.push_back(lastByte);
            }
            
            // Індекси референсів (big-endian, за побудовою завжди > i)
            for (const auto& ref : cell->getReferences()) {
                auto refCell = ref.lock();
                if (!refCell) {
                    throw std::runtime_error("BOC serialization failed: expired cell reference");
                }
                auto it = cellIndices.find(refCell.get());
                if (it == cellIndices.end() || it->second <= i) {
                    throw std::runtime_error("BOC serialization failed: invalid reference order");
                }
                appendBe(result, it->second, sz);
            }
        }
        
        // CRC32-C в кінці (little-endian)
        if (withCrc) {
            uint32_t crc = Boc::calculateCRC32C(result);
            result.push_back(static_cast<uint8_t>(crc & 0xFF));
            result.push_back(static_cast<uint8_t>((crc >> 8) & 0xFF));
            result.push_back(static_cast<uint8_t>((crc >> 16) & 0xFF));
            result.push_back(static_cast<uint8_t>((crc >> 24) & 0xFF));
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
                         std::unordered_set<std::shared_ptr<Cell>>& visited) const {
        // Перевірка чи комірка вже відвідана
        if (visited.find(cell) != visited.end()) {
            return;
        }
        visited.insert(cell);
        cells.push_back(cell);
        
        auto references = cell->getReferences();
        for (const auto& ref : references) {
            if (auto refCell = ref.lock()) {
                collectCells(refCell, cells, visited);
            }
        }
    }
    
    // ------------------------------------------------------------------
    // BocParser — читання стандартного Generic BoC
    // ------------------------------------------------------------------
    
    namespace {
        // Читання big-endian цілого з довільною кількістю байтів
        uint64_t readBe(const std::vector<uint8_t>& d, size_t& off, size_t bytes) {
            if (off + bytes > d.size()) {
                throw std::out_of_range("Not enough data to read integer");
            }
            uint64_t v = 0;
            for (size_t i = 0; i < bytes; ++i) {
                v = (v << 8) | d[off++];
            }
            return v;
        }
    }
    
    BocParser::BocParser(const std::vector<uint8_t>& data) : data_(data), offset_(0) {}

    namespace {
        size_t popcount8(uint32_t mask) {
            size_t n = 0;
            while (mask != 0) {
                n += mask & 1u;
                mask >>= 1;
            }
            return n;
        }

        struct RawCell {
            std::vector<uint8_t> data;
            size_t bitSize = 0;
            std::vector<size_t> refs;
            bool isSpecial = false;
            size_t endOffset = 0;
        };

        RawCell readRawCell(const std::vector<uint8_t>& d, size_t start, size_t limit, size_t refBytes) {
            if (start + 2 > limit || start + 2 > d.size()) {
                throw std::invalid_argument("BOC cell chunk is malformed");
            }
            uint8_t d1 = d[start];
            uint8_t d2 = d[start + 1];
            size_t refsCount = d1 & 0x07;
            bool isSpecial = (d1 & 0x08) != 0;
            bool withHashes = (d1 & 0x10) != 0;
            uint32_t levelMask = d1 >> 5;
            if (refsCount == 7 && withHashes) {
                throw std::invalid_argument("BOC absent cells are not supported");
            }
            if (refsCount > Cell::MAX_REFS) {
                throw std::invalid_argument("BOC cell descriptors out of range");
            }

            size_t hashCount = popcount8(levelMask) + 1;
            size_t dataOffset = 2;
            if (withHashes) {
                dataOffset += hashCount * 32 + hashCount * 2;
            }
            size_t dataLen = static_cast<size_t>(d2 >> 1) + static_cast<size_t>(d2 & 1);
            bool withBits = (d2 & 1) != 0;
            size_t refsOffset = dataOffset + dataLen;
            size_t endOffset = refsOffset + refsCount * refBytes;
            if (start + endOffset > d.size() || start + endOffset > limit) {
                throw std::invalid_argument("BOC cell data is truncated");
            }

            size_t bits = 0;
            std::vector<uint8_t> cellData;
            if (dataLen > 0) {
                cellData.assign(d.begin() + static_cast<std::ptrdiff_t>(start + dataOffset),
                                d.begin() + static_cast<std::ptrdiff_t>(start + dataOffset + dataLen));
            }
            if (!withBits) {
                bits = dataLen * 8;
            } else {
                if (cellData.empty()) {
                    throw std::invalid_argument("BOC cell bit descriptor is empty");
                }
                uint8_t last = cellData.back();
                if ((last & 0x7f) == 0) {
                    throw std::invalid_argument("BOC cell has an overlong completion tag");
                }
                int trailing = 0;
                while (trailing < 8 && ((last >> trailing) & 1) == 0) {
                    ++trailing;
                }
                bits = (dataLen - 1) * 8 + static_cast<size_t>(7 - trailing);
            }
            if (bits > Cell::MAX_BITS) {
                throw std::invalid_argument("BOC cell bit size out of range");
            }

            std::vector<size_t> cellRefs(refsCount);
            size_t pos = start + refsOffset;
            for (size_t j = 0; j < refsCount; ++j) {
                cellRefs[j] = static_cast<size_t>(readBe(d, pos, refBytes));
            }

            RawCell raw;
            raw.data = std::move(cellData);
            raw.bitSize = bits;
            raw.refs = std::move(cellRefs);
            raw.isSpecial = isSpecial;
            raw.endOffset = endOffset;
            return raw;
        }
    }
    
    Boc BocParser::parse() {
        const auto& d = data_;
        if (d.size() < 6) {
            throw std::invalid_argument("Invalid BOC data: too short");
        }
        
        uint32_t magic = (static_cast<uint32_t>(d[0]) << 24) | (static_cast<uint32_t>(d[1]) << 16) |
                         (static_cast<uint32_t>(d[2]) << 8) | d[3];
        
        bool hasIdx = false;
        bool hasCrc = false;
        bool hasCacheBits = false;
        size_t refBytes = 0;
        size_t offBytes = 0;
        
        if (magic == 0xB5EE9C72u || magic == 0x68FF65F3u) {
            uint8_t flagsByte = d[4];
            hasIdx = (flagsByte & 0x80) != 0;
            hasCrc = (flagsByte & 0x40) != 0;
            hasCacheBits = (flagsByte & 0x20) != 0;
            if (((flagsByte >> 3) & 0x03) != 0) {
                throw std::invalid_argument("BOC header flags must be zero");
            }
            refBytes = flagsByte & 0x07;
            offBytes = d[5];
        } else if (magic == 0x73F5E303u || magic == 0x91B1BA9Eu) {
            throw std::invalid_argument("Unsupported BOC variant (index-only/chained)");
        } else {
            throw std::invalid_argument("Invalid BOC magic");
        }
        
        if (refBytes < 1 || refBytes > 4 || offBytes < 1 || offBytes > 8) {
            throw std::invalid_argument("BOC size fields are out of range");
        }
        if (hasCacheBits && !hasIdx) {
            throw std::invalid_argument("BOC cache bits require an index");
        }
        
        size_t pos = 6;
        size_t cellCount = static_cast<size_t>(readBe(d, pos, refBytes));
        size_t rootCount = static_cast<size_t>(readBe(d, pos, refBytes));
        /*absent*/ readBe(d, pos, refBytes);
        size_t totCellsSize = static_cast<size_t>(readBe(d, pos, offBytes));
        if (cellCount == 0 || rootCount == 0 || rootCount > cellCount) {
            throw std::invalid_argument("BOC cell or root count is invalid");
        }
        
        std::vector<size_t> rootIndices(rootCount);
        for (size_t i = 0; i < rootCount; ++i) {
            rootIndices[i] = static_cast<size_t>(readBe(d, pos, refBytes));
            if (rootIndices[i] >= cellCount) {
                throw std::invalid_argument("BOC root index is out of range");
            }
        }
        
        std::vector<size_t> ends(cellCount);
        if (hasIdx) {
            for (size_t i = 0; i < cellCount; ++i) {
                uint64_t raw = readBe(d, pos, offBytes);
                if (hasCacheBits) {
                    raw /= 2;
                }
                ends[i] = static_cast<size_t>(raw);
            }
        }
        
        if (pos + totCellsSize > d.size() || (hasCrc && pos + totCellsSize + 4 > d.size())) {
            throw std::invalid_argument("BOC data is truncated");
        }
        if (hasCrc) {
            std::vector<uint8_t> body(d.begin(), d.begin() + static_cast<std::ptrdiff_t>(pos + totCellsSize));
            uint32_t storedCrc = static_cast<uint32_t>(d[pos + totCellsSize])
                               | (static_cast<uint32_t>(d[pos + totCellsSize + 1]) << 8)
                               | (static_cast<uint32_t>(d[pos + totCellsSize + 2]) << 16)
                               | (static_cast<uint32_t>(d[pos + totCellsSize + 3]) << 24);
            if (Boc::calculateCRC32C(body) != storedCrc) {
                throw std::invalid_argument("BOC CRC32-C mismatch");
            }
        }
        
        size_t dataStart = pos;
        size_t dataEnd = dataStart + totCellsSize;
        
        if (!hasIdx) {
            size_t cursor = dataStart;
            for (size_t i = 0; i < cellCount; ++i) {
                RawCell peeked = readRawCell(d, cursor, dataEnd, refBytes);
                cursor += peeked.endOffset;
                ends[i] = cursor - dataStart;
            }
            if (cursor != dataEnd) {
                throw std::invalid_argument("BOC cells do not fill tot_cells_size");
            }
        }
        
        struct Kept {
            std::vector<uint8_t> data;
            size_t bitSize;
            std::vector<size_t> refs;
            bool isSpecial;
        };
        std::vector<Kept> raw(cellCount);
        
        for (size_t i = 0; i < cellCount; ++i) {
            size_t chunkStart = dataStart + (i == 0 ? 0 : ends[i - 1]);
            size_t chunkEnd = dataStart + ends[i];
            if (chunkEnd < chunkStart || chunkEnd > dataEnd) {
                throw std::invalid_argument("BOC index is not monotonic");
            }
            RawCell cell = readRawCell(d, chunkStart, chunkEnd, refBytes);
            if (chunkStart + cell.endOffset != chunkEnd) {
                throw std::invalid_argument("BOC cell does not fill its index slot");
            }
            for (size_t ref : cell.refs) {
                if (ref <= i || ref >= cellCount) {
                    throw std::invalid_argument("BOC reference index violates topological order");
                }
            }
            raw[i].data = std::move(cell.data);
            raw[i].bitSize = cell.bitSize;
            raw[i].refs = std::move(cell.refs);
            raw[i].isSpecial = cell.isSpecial;
        }
        
        // Будуємо комірки від останньої до першої (референси завжди попереду)
        std::vector<std::shared_ptr<Cell>> cells(cellCount);
        for (size_t i = cellCount; i-- > 0; ) {
            std::vector<std::shared_ptr<Cell>> refs;
            refs.reserve(raw[i].refs.size());
            for (size_t r : raw[i].refs) {
                if (!cells[r]) {
                    throw std::invalid_argument("BOC reference to unparsed cell");
                }
                refs.push_back(cells[r]);
            }
            cells[i] = std::make_shared<Cell>(raw[i].data, raw[i].bitSize, refs, raw[i].isSpecial);
        }
        
        std::shared_ptr<Cell> root;
        if (rootCount > 0 && rootIndices[0] < cellCount) {
            root = cells[rootIndices[0]];
        } else if (cellCount > 0) {
            root = cells[cellCount - 1];
        } else {
            root = std::make_shared<Cell>();
        }
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
    
    // CRC32-C (Castagnoli), віддзеркалений поліном 0x82F63B78 — вимагається форматом BoC TON
    uint32_t Boc::calculateCRC32C(const std::vector<uint8_t>& data) {
        static uint32_t crcTable[256];
        static bool tableInitialized = false;
        
        if (!tableInitialized) {
            for (int i = 0; i < 256; i++) {
                uint32_t crc = static_cast<uint32_t>(i);
                for (int j = 0; j < 8; j++) {
                    if (crc & 1) {
                        crc = (crc >> 1) ^ 0x82F63B78u;
                    } else {
                        crc >>= 1;
                    }
                }
                crcTable[i] = crc;
            }
            tableInitialized = true;
        }
        
        uint32_t crc = 0xFFFFFFFFu;
        for (uint8_t byte : data) {
            crc = (crc >> 8) ^ crcTable[(crc ^ byte) & 0xFF];
        }
        
        return crc ^ 0xFFFFFFFFu;
    }
    
    BocBuilder::BocBuilder(std::shared_ptr<Cell> root) : root_(root) {}
    
    std::vector<uint8_t> BocBuilder::build(bool hasIdx, bool hashCRC) {
        // Реалізація побудови BOC
        if (!root_) {
            return std::vector<uint8_t>();
        }
        
        Boc boc(root_);
        return boc.serialize(hasIdx, hashCRC);
    }
}
