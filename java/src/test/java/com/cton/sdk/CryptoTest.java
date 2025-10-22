// CryptoTest.java - Тести для Crypto
// Author: Андрій Будильников (Sparky)

package com.cton.sdk;

import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import org.junit.jupiter.api.Test;

/**
 * Тести для Crypto
 */
public class CryptoTest {
    
    @Test
    public void testGeneratePrivateKey() {
        // Генерація приватного ключа
        Crypto.PrivateKey privateKey = Crypto.PrivateKey.generate();
        
        // Перевіряємо, що ключ створено
        assertNotNull(privateKey);
        
        // Отримуємо дані ключа
        byte[] keyData = privateKey.getData();
        assertNotNull(keyData);
        assertEquals(32, keyData.length);
    }
    
    @Test
    public void testPrivateKeyToPublicKey() {
        // Генерація приватного ключа
        Crypto.PrivateKey privateKey = Crypto.PrivateKey.generate();
        
        // Отримання публічного ключа
        Crypto.PublicKey publicKey = privateKey.getPublicKey();
        
        // Перевіряємо, що ключ створено
        assertNotNull(publicKey);
        
        // Отримуємо дані ключа
        byte[] keyData = publicKey.getData();
        assertNotNull(keyData);
        assertEquals(32, keyData.length);
    }
    
    @Test
    public void testGenerateMnemonic() {
        // Генерація мнемонічної фрази
        List<String> mnemonic = Crypto.generateMnemonic();
        
        // Перевіряємо, що фраза створена
        assertNotNull(mnemonic);
        assertEquals(24, mnemonic.size());
    }
    
    @Test
    public void testMnemonicToPrivateKey() {
        // Генерація мнемонічної фрази
        List<String> mnemonic = Crypto.generateMnemonic();
        
        // Створення приватного ключа з мнемоніки
        Crypto.PrivateKey privateKey = Crypto.mnemonicToPrivateKey(mnemonic);
        
        // Перевіряємо, що ключ створено
        assertNotNull(privateKey);
        
        // Отримуємо дані ключа
        byte[] keyData = privateKey.getData();
        assertNotNull(keyData);
        assertEquals(32, keyData.length);
    }
}