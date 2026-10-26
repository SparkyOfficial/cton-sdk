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
     * Додати один біт (0 або 1)
     * @param bit значення біта
     * @return this для ланцюгових викликів
     */
    public CellBuilder storeBit(boolean bit) {
        cell.storeUInt(1, bit ? 1 : 0);
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
        cell.storeUInt(2, 2); // addr_std$10
        
        // Зберігаємо anycast:(Maybe Int8) — відсутній (біт 0), як у стандарті TON
        // Store anycast:(Maybe Int8) = Nothing (single 0 bit)
        cell.storeUInt(1, 0);
        
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
        // Реалізація серіалізації Grams (varuint16) згідно з TL-B TON:
        //   Grams$_ (len##4) value:(len * 8) Bits
        // де len — кількість значущих байтів, a value — big-endian.
        // Implementation of Grams (varuint16) per TON TL-B.
        if (amount == null || amount.signum() <= 0) {
            // Нульове значення: len = 0
            cell.storeUInt(4, 0);
            return this;
        }
        
        // Кількість значущих байтів (big-endian, без знакового байта)
        byte[] full = amount.toByteArray(); // може містити провідний 0x00 для додатних чисел
        int start = 0;
        while (start < full.length - 1 && full[start] == 0) {
            start++;
        }
        int len = full.length - start;
        if (len > 15) {
            throw new IllegalArgumentException("Coins value too large for Grams (max 15 bytes)");
        }
        
        // Довжина у байтах (4 біти)
        cell.storeUInt(4, len);
        
        // Значення big-endian
        for (int i = start; i < full.length; i++) {
            cell.storeUInt(8, full[i] & 0xFF);
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