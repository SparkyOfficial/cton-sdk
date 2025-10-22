// CellBuilderTest.java - Тести для CellBuilder
// Author: Андрій Будильников (Sparky)

package com.cton.sdk;

import java.math.BigInteger;

import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;
import org.junit.jupiter.api.Test;

/**
 * Тести для CellBuilder
 */
public class CellBuilderTest {
    
    @Test
    public void testBuildSimpleCell() {
        // Створюємо просту комірку з кількома значеннями
        Cell cell = new CellBuilder()
            .storeUInt(32, 0x12345678)
            .storeInt(16, -100)
            .storeBytes(new byte[]{0x01, 0x02, 0x03, 0x04})
            .build();
        
        // Перевіряємо, що комірка створена
        assertNotNull(cell);
        assertTrue(cell.getBitSize() > 0);
        
        // Перевіряємо, що дані присутні
        byte[] data = cell.getData();
        assertNotNull(data);
        assertTrue(data.length > 0);
    }
    
    @Test
    public void testBuildMessageBody() {
        // Приклад з опису проекту
        long opCode = 0x12345678;
        Address destinationAddress = new Address("-1:5555555555555555555555555555555555555555555555555555555555555555");
        BigInteger amount = BigInteger.valueOf(1000000000L); // 1 TON в нанотоні
        
        Cell messageBody = new CellBuilder()
            .storeUInt(32, opCode)
            .storeAddress(destinationAddress)
            .storeCoins(amount)
            .build();
        
        // Перевіряємо, що комірка створена
        assertNotNull(messageBody);
        assertTrue(messageBody.getBitSize() > 0);
    }
    
    @Test
    public void testCellWithReference() {
        // Створюємо внутрішню комірку
        Cell innerCell = new CellBuilder()
            .storeUInt(8, 0xFF)
            .build();
        
        // Створюємо зовнішню комірку з посиланням на внутрішню
        Cell outerCell = new CellBuilder()
            .storeUInt(32, 0x12345678)
            .storeRef(innerCell)
            .build();
        
        // Перевіряємо, що обидві комірки створені
        assertNotNull(innerCell);
        assertNotNull(outerCell);
    }
}