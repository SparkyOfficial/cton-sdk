// Slice.cpp - читання бітів комірки TON (MSB first)

#include "../include/Slice.h"
#include <stdexcept>

namespace cton {

    Slice::Slice(const Cell& cell)
        : cell_(cell.clone()), bitPos_(0), refPos_(0) {
        data_ = cell_->getData();
    }

    uint64_t Slice::readBits(size_t count) {
        if (count > 64) {
            throw std::invalid_argument("Slice can load at most 64 bits at once");
        }
        if (bitPos_ + count > cell_->getBitSize()) {
            throw std::out_of_range("Slice underflow");
        }
        uint64_t value = 0;
        for (size_t i = 0; i < count; ++i) {
            size_t pos = bitPos_ + i;
            uint8_t bit = static_cast<uint8_t>((data_[pos / 8] >> (7 - (pos % 8))) & 1);
            value = (value << 1) | bit;
        }
        bitPos_ += count;
        return value;
    }

    uint64_t Slice::loadUInt(size_t bits) {
        if (bits == 0) {
            return 0;
        }
        return readBits(bits);
    }

    int64_t Slice::loadInt(size_t bits) {
        if (bits == 0 || bits > 64) {
            throw std::invalid_argument("loadInt supports 1..64 bits");
        }
        uint64_t value = readBits(bits);
        if (bits < 64 && ((value >> (bits - 1)) & 1ULL)) {
            uint64_t sign = ~((1ULL << bits) - 1ULL);
            value |= sign;
        }
        return static_cast<int64_t>(value);
    }

    std::vector<uint8_t> Slice::loadBytes(size_t length) {
        std::vector<uint8_t> out(length);
        for (size_t i = 0; i < length; ++i) {
            out[i] = static_cast<uint8_t>(readBits(8));
        }
        return out;
    }

    std::optional<Address> Slice::loadAddress() {
        uint64_t tag = readBits(2);
        if (tag == 0) {
            return std::nullopt;
        }
        if (tag == 1) {
            uint64_t len = readBits(9);
            if (len > 0) {
                readBits(len > 64 ? 64 : len);
                size_t left = static_cast<size_t>(len > 64 ? len - 64 : 0);
                while (left > 0) {
                    size_t step = left > 64 ? 64 : left;
                    readBits(step);
                    left -= step;
                }
            }
            throw std::invalid_argument("External address is not a standard MsgAddressInt");
        }

        bool anycast = readBits(1) != 0;
        if (anycast) {
            uint64_t depth = readBits(5);
            size_t left = static_cast<size_t>(depth);
            while (left > 0) {
                size_t step = left > 64 ? 64 : left;
                readBits(step);
                left -= step;
            }
        }

        if (tag == 2) {
            int64_t workchain = loadInt(8);
            std::vector<uint8_t> hash = loadBytes(32);
            return Address(static_cast<int8_t>(workchain), hash);
        }

        uint64_t addrLen = readBits(9);
        int64_t workchain = loadInt(32);
        if (workchain < -128 || workchain > 127 || addrLen != 256) {
            size_t left = static_cast<size_t>(addrLen);
            while (left > 0) {
                size_t step = left > 64 ? 64 : left;
                readBits(step);
                left -= step;
            }
            throw std::invalid_argument("addr_var does not fit workchain:int8 and 256-bit hash");
        }
        std::vector<uint8_t> hash = loadBytes(32);
        return Address(static_cast<int8_t>(workchain), hash);
    }

    std::vector<uint8_t> Slice::loadCoins() {
        size_t len = static_cast<size_t>(readBits(4));
        if (len == 0) {
            return {};
        }
        if (len > 15) {
            throw std::invalid_argument("VarUInteger 16 length is out of range");
        }
        return loadBytes(len);
    }

    std::shared_ptr<Cell> Slice::loadRef() {
        if (refPos_ >= cell_->getRefsCount()) {
            throw std::out_of_range("Slice has no more references");
        }
        auto refs = cell_->getReferences();
        auto ref = refs[refPos_].lock();
        if (!ref) {
            throw std::runtime_error("Slice reference expired");
        }
        ++refPos_;
        return ref;
    }

    size_t Slice::remainingBits() const {
        size_t total = cell_->getBitSize();
        return bitPos_ >= total ? 0 : total - bitPos_;
    }

    size_t Slice::remainingRefs() const {
        size_t total = cell_->getRefsCount();
        return refPos_ >= total ? 0 : total - refPos_;
    }

}
