// Cell.h - представлення комірки TON
// Author: Андрій Будильников (Sparky)
// Cell - основна одиниця даних в TON

#ifndef CTON_CELL_H
#define CTON_CELL_H

#include <vector>
#include <memory>
#include <cstdint>
#include <string>

// Export definitions for Windows DLL
#ifdef _WIN32
    #ifdef CTON_SDK_CORE_EXPORTS
        #define CTON_SDK_CORE_API __declspec(dllexport)
    #else
        #define CTON_SDK_CORE_API __declspec(dllimport)
    #endif
#else
    #define CTON_SDK_CORE_API
#endif

namespace cton {
    
    // Forward declaration
    class CTON_SDK_CORE_API CellBuilder;
    
    /**
     * @brief Представляє комірку TON - основну одиницю даних
     * 
     * Cell містить до 1023 бітів даних і до 4 посилань на інші комірки
     */
    class CTON_SDK_CORE_API Cell {
        friend class CellBuilder;
        
    public:
        // Константи для обмежень комірки
        static const size_t MAX_BITS = 1023;  // Максимальна кількість бітів у комірці
        static const size_t MAX_REFS = 4;     // Максимальна кількість посилань
        
        /**
         * @brief Конструктор за замовчуванням
         */
        Cell();
        
        /**
         * @brief Конструктор з даних
         * @param data бінарні дані
         * @param bitSize розмір даних у бітах
         * @param references посилання на інші комірки
         * @param isSpecial чи є комірка спеціальною
         */
        Cell(const std::vector<uint8_t>& data, 
             size_t bitSize, 
             const std::vector<std::shared_ptr<Cell>>& references,
             bool isSpecial = false);
        
        /**
         * @brief Зберегти беззнакове ціле (big-endian, MSB first) у кінець комірки
         * @param bitCount кількість бітів (0..64)
         * @param value значення
         */
        void storeUInt(size_t bitCount, uint64_t value);
        
        /**
         * @brief Зберегти знакове ціле (two's complement) у кінець комірки
         * @param bitCount кількість бітів
         * @param value значення
         */
        void storeInt(size_t bitCount, int64_t value);
        
        /**
         * @brief Зберегти байти у кінець комірки
         * @param bytes вектор байтів
         */
        void storeBytes(const std::vector<uint8_t>& bytes);
        
        /**
         * @brief Зберегти один біт (0 або 1) у кінець комірки
         * @param bit значення біта
         */
        void storeBit(bool bit);
        
        /**
         * @brief Додати посилання на комірку
         * @param cell комірка для посилання
         */
        void addReference(std::shared_ptr<Cell> cell);
        
        /**
         * @brief Отримати дані комірки
         * @return вектор байтів
         */
        std::vector<uint8_t> getData() const;
        
        /**
         * @brief Отримати розмір даних у бітах
         * @return розмір у бітах
         */
        size_t getBitSize() const;
        
        /**
         * @brief Отримати посилання
         * @return вектор посилань
         */
        std::vector<std::weak_ptr<Cell>> getReferences() const;
        
        /**
         * @brief Отримати кількість посилань
         * @return кількість посилань
         */
        size_t getRefsCount() const;
        
        /**
         * @brief Перевірити чи є комірка спеціальною
         * @return true якщо спеціальна
         */
        bool isSpecial() const;
        
        /**
         * @brief Обчислити representation hash комірки.
         * sha256(d1 d2 | data з completion-бітом | depth(2) кожного ref | hash(32) кожного ref).
         * @return 32-байтовий хеш
         */
        std::vector<uint8_t> representationHash() const;

        /**
         * @brief Глибина комірки: 0 без посилань, інакше 1 + максимум глибин дітей.
         */
        uint16_t depth() const;

        /**
         * @brief Глибока копія дерева. Нова комірка не ділить володіння з оригіналом.
         */
        std::shared_ptr<Cell> clone() const;
        
    private:
        std::vector<uint8_t> data_;
        size_t bitSize_;
        std::vector<std::shared_ptr<Cell>> references_;
        bool isSpecial_;
        
        /**
         * @brief Перевірити чи можна зберегти біти
         * @param bitCount кількість бітів для збереження
         */
        void checkCapacity(size_t bitCount);
        
        /**
         * @brief Дописати біти (MSB first) у кінець комірки
         * @param count кількість бітів (0..64)
         * @param value значення
         */
        void appendBits(size_t count, uint64_t value);
    };
    
    /**
     * @brief Будівельник для створення комірок
     */
    class CTON_SDK_CORE_API CellBuilder {
    public:
        /**
         * @brief Конструктор за замовчуванням
         */
        CellBuilder();
        
        /**
         * @brief Зберегти беззнакове ціле
         * @param bitCount кількість бітів
         * @param value значення
         * @return посилання на себе для ланцюжкових викликів
         */
        CellBuilder& storeUInt(size_t bitCount, uint64_t value);
        
        /**
         * @brief Зберегти один біт
         * @param bit значення біта
         * @return посилання на себе для ланцюжкових викликів
         */
        CellBuilder& storeBit(bool bit);
        
        /**
         * @brief Зберегти знакове ціле
         * @param bitCount кількість бітів
         * @param value значення
         * @return посилання на себе для ланцюжкових викликів
         */
        CellBuilder& storeInt(size_t bitCount, int64_t value);
        
        /**
         * @brief Зберегти байти
         * @param bytes вектор байтів
         * @return посилання на себе для ланцюжкових викликів
         */
        CellBuilder& storeBytes(const std::vector<uint8_t>& bytes);
        
        /**
         * @brief Зберегти посилання на комірку
         * @param cell комірка для посилання
         * @return посилання на себе для ланцюжкових викликів
         */
        CellBuilder& storeRef(std::shared_ptr<Cell> cell);
        
        /**
         * @brief Побудувати комірку
         * @return створена комірка
         */
        std::shared_ptr<Cell> build();
        
    private:
        std::vector<uint8_t> buffer_;
        size_t bitOffset_;
        std::vector<std::shared_ptr<Cell>> references_;
    };
    
}

#endif // CTON_CELL_H