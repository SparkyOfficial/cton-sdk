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
        // TODO: реалізувати серіалізацію адреси в комірку
        // Поки що просто зберігаємо хеш-частину
        cell.storeBytes(address.getHashPart());
        return this;
    }
    
    /**
     * Додати монети (TON) до комірки
     * @param amount кількість монет у нанотоні
     * @return this для ланцюгових викликів
     */
    public CellBuilder storeCoins(BigInteger amount) {
        // TODO: реалізувати серіалізацію монет в комірку
        // Поки що просто зберігаємо як 64-бітне число
        cell.storeUInt(64, amount.longValue());
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