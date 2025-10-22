// BocTest.java - Тести для Boc
// Author: Андрій Будильников (Sparky)

package com.cton.sdk;

import static org.junit.jupiter.api.Assertions.assertNotNull;
import org.junit.jupiter.api.Test;

/**
 * Тести для Boc
 */
public class BocTest {
    
    @Test
    public void testCreateBoc() {
        // Створення порожнього BOC
        Boc boc = new Boc();
        
        // Перевіряємо, що BOC створено
        assertNotNull(boc);
    }
    
    @Test
    public void testCreateBocWithRootCell() {
        // Створення комірки
        Cell cell = new CellBuilder()
            .storeUInt(32, 0x12345678)
            .build();
        
        // Створення BOC з кореневою коміркою
        Boc boc = new Boc(cell);
        
        // Перевіряємо, що BOC створено
        assertNotNull(boc);
        
        // Отримання кореневої комірки
        Cell rootCell = boc.getRoot();
        assertNotNull(rootCell);
    }
    
    @Test
    public void testSerializeDeserialize() {
        // Створення комірки
        Cell cell = new CellBuilder()
            .storeUInt(32, 0x12345678)
            .storeBytes(new byte[]{0x01, 0x02, 0x03, 0x04})
            .build();
        
        // Створення BOC з кореневою коміркою
        Boc boc = new Boc(cell);
        
        // Серіалізація
        byte[] serialized = boc.serialize(true, true);
        assertNotNull(serialized);
        
        // Десеріалізація
        Boc deserializedBoc = Boc.deserialize(serialized);
        assertNotNull(deserializedBoc);
        
        // Отримання кореневої комірки
        Cell rootCell = deserializedBoc.getRoot();
        assertNotNull(rootCell);
    }
    
    @Test
    public void testSetGetRoot() {
        // Створення BOC
        Boc boc = new Boc();
        
        // Створення комірки
        Cell cell = new CellBuilder()
            .storeUInt(32, 0x12345678)
            .build();
        
        // Встановлення кореневої комірки
        boc.setRoot(cell);
        
        // Отримання кореневої комірки
        Cell rootCell = boc.getRoot();
        assertNotNull(rootCell);
    }
}