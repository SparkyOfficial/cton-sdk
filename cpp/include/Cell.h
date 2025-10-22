// Cell.h - основна структура даних для TON
// Author: Андрій Будильников (Sparky)
// Основна структура даних TON - Cell (комірка)
// представляє вузол в дереві Bag of Cells (BOC)

#ifndef CTON_CELL_H
#define CTON_CELL_H

#include <vector>
#include <memory>
#include <cstdint>

namespace cton {
    
    // Forward declaration
    class CellBuilder;
    
    /**
     * @brief Представляє комірку в структурі Bag of Cells (BOC)
     * 
     * Cell - це основна одиниця даних в TON, яка може містити:
     * - до 1023 бітів даних
     * - до 4 референсів на інші комірки
     * - спеціальні прапорці (depth, special cells, etc.)
     */
    class Cell {
    public:
        // Максимальна кількість бітів в комірці
        static const size_t MAX_BITS = 1023;
        // Максимальна кількість референсів
        static const size_t MAX_REFS = 4;
        
        /**
         * @brief Конструктор за замовчуванням
         */
        Cell();
        
        /**
         * @brief Деструктор
         */
        ~Cell();
        
        /**
         * @brief Отримати дані з комірки
         * @return вектор байтів з даними
         */
        std::vector<uint8_t> getData() const;
        
        /**
         * @brief Отримати кількість бітів в комірці
         * @return кількість бітів
         */
        size_t getBitSize() const;
        
        /**
         * @brief Отримати референси на інші комірки
         * @return вектор слабких посилань на комірки
         */
        std::vector<std::weak_ptr<Cell>> getReferences() const;
        
        /**
         * @brief Перевірити чи є комірка спеціальною
         * @return true якщо комірка спеціальна
         */
        bool isSpecial() const;
        
    private:
        // Дані комірки
        std::vector<uint8_t> data_;
        size_t bitSize_;
        
        // Референси на інші комірки
        std::vector<std::weak_ptr<Cell>> references_;
        
        // Спеціальні прапорці
        bool special_;
        
        // Глибина дерева для цієї комірки
        uint16_t depth_;
        
        // Приватний конструктор для CellBuilder
        Cell(const std::vector<uint8_t>& data, size_t bitSize, 
             const std::vector<std::weak_ptr<Cell>>& references, bool special);
        
        // Дружній клас для побудови комірок
        friend class CellBuilder;
    };
    
    /**
     * @brief Будівельник комірок для зручного створення Cell
     * 
     * CellBuilder дозволяє зручно створювати комірки,
     * додаючи до них дані та референси
     */
    class CellBuilder {
    public:
        /**
         * @brief Конструктор за замовчуванням
         */
        CellBuilder();
        
        /**
         * @brief Додати беззнакове ціле число до комірки
         * @param bits кількість бітів для зберігання
         * @param value значення для зберігання
         * @return посилання на цей будівельник для ланцюгових викликів
         */
        CellBuilder& storeUInt(size_t bits, uint64_t value);
        
        /**
         * @brief Додати знакове ціле число до комірки
         * @param bits кількість бітів для зберігання
         * @param value значення для зберігання
         * @return посилання на цей будівельник для ланцюгових викликів
         */
        CellBuilder& storeInt(size_t bits, int64_t value);
        
        /**
         * @brief Додати байти до комірки
         * @param data дані для зберігання
         * @return посилання на цей будівельник для ланцюгових викликів
         */
        CellBuilder& storeBytes(const std::vector<uint8_t>& data);
        
        /**
         * @brief Додати референс на іншу комірку
         * @param cell комірка для додавання як референс
         * @return посилання на цей будівельник для ланцюгових викликів
         */
        CellBuilder& storeRef(std::shared_ptr<Cell> cell);
        
        /**
         * @brief Побудувати комірку з накопичених даних
         * @return спільне посилання на створену комірку
         */
        std::shared_ptr<Cell> build();
        
    private:
        // Накопичувач даних
        std::vector<uint8_t> buffer_;
        size_t bitOffset_;
        
        // Накопичувач референсів
        std::vector<std::shared_ptr<Cell>> references_;
    };
}

#endif // CTON_CELL_H