// CellBuilder.java - Fluent API для створення Cell
// Author: Андрій Будильников (Sparky)

package com.cton.sdk;

import java.math.BigInteger;

/**
 * Fluent API для створення Cell
 * 
 * Дозволяє зручно створювати комірки в ланцюжному стилі:
 * Cell messageBody = new CellBuilder()
 *     .storeUInt(32, opCode)
 *     .storeAddress(destinationAddress)
 *     .storeCoins(amount)
 *     .build();
 */
public class CellBuilder {
    private Cell cell;
    
    /**
     * Конструктор за замовчуванням
     */
    public CellBuilder() {
        this.cell = new Cell();
    }
    
    /**
     * Додати беззнакове ціле число до комірки
     * @param bits кількість бітів для зберігання
     * @param value значення для зберігання
     * @return this для ланцюгових викликів
     */
    public CellBuilder storeUInt(int bits, long value) {
        cell.storeUInt(bits, value);
        return this;
    }
    
    /**
     * Додати знакове ціле число до комірки
     * @param bits кількість бітів для зберігання
     * @param value значення для зберігання
     * @return this для ланцюгових викликів
     */
    public CellBuilder storeInt(int bits, long value) {
        cell.storeInt(bits, value);
        return this;
    }
    
    /**
     * Додати байти до комірки
     * @param data дані для зберігання
     * @return this для ланцюгових викликів
     */
    public CellBuilder storeBytes(byte[] data) {
        cell.storeBytes(data);
        return this;
    }
    
    /**
     * Додати референс на іншу комірку
     * @param cell комірка для додавання як референс
     * @return this для ланцюгових викликів
     */
    public CellBuilder storeRef(Cell cell) {
        this.cell.storeRef(cell);
        return this;
    }
    
    /**
     * Додати адресу до комірки
     * @param address адреса для зберігання
     * @return this для ланцюгових викликів
     */
    public CellBuilder storeAddress(Address address) {
        // Реалізація серіалізації адреси в комірку
        // Implementation of address serialization to cell
        // Реализация сериализации адреса в ячейку
        
        if (address == null || !address.isValid()) {
            // Зберігаємо нульову адресу
            // Store null address
            // Сохраняем нулевой адрес
            cell.storeUInt(2, 0); // None адреса
            return this;
        }
        
        // Зберігаємо тип адреси (addr_std)
        // Store address type (addr_std)
        // Сохраняем тип адреса (addr_std)
        cell.storeUInt(2, 2); // addr_std
        
        // Зберігаємо workchain
        // Store workchain
        // Сохраняем workchain
        cell.storeInt(8, address.getWorkchain());
        
        // Зберігаємо хеш-частину
        // Store hash part
        // Сохраняем хеш-часть
        cell.storeBytes(address.getHashPart());
        
        return this;
    }

    /**
     * Додати монети (TON) до комірки
     * @param amount кількість монет у нанотоні
     * @return this для ланцюгових викликів
     */
    public CellBuilder storeCoins(BigInteger amount) {
        // Реалізація серіалізації монет в комірку
        // Implementation of coin serialization to cell
        // Реализация сериализации монет в ячейку
        
        if (amount == null || amount.compareTo(BigInteger.ZERO) < 0) {
            // Зберігаємо нуль
            // Store zero
            // Сохраняем ноль
            cell.storeUInt(4, 0);
            return this;
        }
        
        // Визначаємо кількість байтів для зберігання
        // Determine number of bytes to store
        // Определяем количество байтов для хранения
        int bitLength = amount.bitLength();
        int bytesNeeded = (bitLength + 7) / 8;
        
        if (bytesNeeded == 0) {
            // Нульове значення
            // Zero value
            // Нулевое значение
            cell.storeUInt(4, 0);
            return this;
        }
        
        // Обчислюємо кількість байтів для кодування
        // Calculate number of bytes for encoding
        // Вычисляем количество байтов для кодирования
        int encodingBytes = Math.min(bytesNeeded, 127); // Максимум 127 байтів
        
        // Зберігаємо довжину (кількість байтів)
        // Store length (number of bytes)
        // Сохраняем длину (количество байтов)
        cell.storeUInt(4, encodingBytes * 2 + 1); // Непарне число для позначення наявності байтів
        
        // Конвертуємо в байти у форматі big-endian
        // Convert to bytes in big-endian format
        // Конвертируем в байты в формате big-endian
        byte[] amountBytes = amount.toByteArray();
        
        // Зберігаємо байти
        // Store bytes
        // Сохраняем байты
        for (int i = amountBytes.length - 1; i >= 0; i--) {
            cell.storeUInt(8, amountBytes[i] & 0xFF);
        }
        
        return this;
    }
    
    /**
     * Побудувати комірку з накопичених даних
     * @return створена комірка
     */
    public Cell build() {
        // Повертаємо готову комірку
        return cell;
    }
}