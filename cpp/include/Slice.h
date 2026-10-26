// Slice.h - курсор читання комірки TON
// Author: Андрій Будильников (Sparky)
// Slice читає біти та посилання, які Cell зберігає MSB first.

#ifndef CTON_SLICE_H
#define CTON_SLICE_H

#include "Cell.h"
#include "Address.h"
#include <cstdint>
#include <cstddef>
#include <memory>
#include <optional>
#include <vector>

namespace cton {

    /**
     * @brief Курсор по бітах і посиланнях комірки.
     *
     * Аналог TVM Slice: loadUInt / loadInt / loadBytes / loadAddress / loadCoins / loadRef.
     * Копіює дерево комірки, тому живе незалежно від вихідного Cell.
     */
    class CTON_SDK_CORE_API Slice {
    public:
        explicit Slice(const Cell& cell);

        uint64_t loadUInt(size_t bits);
        int64_t loadInt(size_t bits);
        std::vector<uint8_t> loadBytes(size_t length);

        /**
         * addr_none повертає nullopt. addr_std повертає адресу.
         * Зовнішня та addr_var адреси, які не вміщаються в workchain:int8 + hash:256, кидають виняток.
         */
        std::optional<Address> loadAddress();

        /**
         * VarUInteger 16 (Grams). Порожній вектор означає 0.
         * Байти — big-endian без провідних нулів, крім самого нуля.
         */
        std::vector<uint8_t> loadCoins();

        std::shared_ptr<Cell> loadRef();

        size_t remainingBits() const;
        size_t remainingRefs() const;

    private:
        std::shared_ptr<Cell> cell_;
        std::vector<uint8_t> data_;
        size_t bitPos_;
        size_t refPos_;

        uint64_t readBits(size_t count);
    };

}

#endif // CTON_SLICE_H
