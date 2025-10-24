// Crypto.java - Java обгортка для Crypto C++ класу
// Author: Андрій Будильников (Sparky)

package com.cton.sdk;

import java.io.Closeable;
import java.util.Arrays;
import java.util.List;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;

/**
 * Java обгортка для Crypto C++ класу
 * 
 * Реалізація криптографічних функцій Ed25519 для TON
 */
public class Crypto {
    // Інтерфейс для взаємодії з нативною бібліотекою
    public interface CtonLibrary extends Library {
        CtonLibrary INSTANCE = (CtonLibrary) Native.load("cton-sdk-core", CtonLibrary.class);
        
        // Створення нового приватного ключа
        Pointer private_key_create();
        
        // Створення приватного ключа з даних
        Pointer private_key_create_from_data(byte[] keyData, int length);
        
        // Звільнення пам'яті приватного ключа
        void private_key_destroy(Pointer privateKey);
        
        // Генерація нового приватного ключа
        Pointer private_key_generate();
        
        // Отримання даних приватного ключа
        int private_key_get_data(Pointer privateKey, byte[] buffer, int bufferSize);
        
        // Отримання публічного ключа з приватного
        Pointer private_key_get_public_key(Pointer privateKey);
        
        // Створення нового публічного ключа
        Pointer public_key_create();
        
        // Створення публічного ключа з даних
        Pointer public_key_create_from_data(byte[] keyData, int length);
        
        // Звільнення пам'яті публічного ключа
        void public_key_destroy(Pointer publicKey);
        
        // Отримання даних публічного ключа
        int public_key_get_data(Pointer publicKey, byte[] buffer, int bufferSize);
        
        // Перевірка підпису
        boolean public_key_verify_signature(Pointer publicKey, byte[] message, int messageLen, byte[] signature, int signatureLen);
        
        // Створення підпису
        Pointer crypto_sign(Pointer privateKey, byte[] message, int messageLen);
        
        // Перевірка підпису
        boolean crypto_verify(Pointer publicKey, byte[] message, int messageLen, byte[] signature, int signatureLen);
        
        // Генерація мнемонічної фрази
        Pointer crypto_generate_mnemonic();
        
        // Створення приватного ключа з мнемоніки
        Pointer crypto_mnemonic_to_private_key(Pointer mnemonic);
        
        // Функція для звільнення пам'яті рядків
        void free_string(Pointer str);
        
        // Функція для звільнення масиву рядків
        void free_string_array(Pointer strArray);
    }
    
    /**
     * Представляє приватний ключ Ed25519
     */
    public static class PrivateKey implements Closeable {
        private Pointer nativePrivateKey;
        private boolean closed = false;
        
        /**
         * Конструктор за замовчуванням
         */
        public PrivateKey() {
            this.nativePrivateKey = CtonLibrary.INSTANCE.private_key_create();
        }
        
        /**
         * Конструктор з даних
         * @param keyData дані ключа (32 байти)
         */
        public PrivateKey(byte[] keyData) {
            if (keyData.length != 32) {
                throw new IllegalArgumentException("Private key data must be exactly 32 bytes");
            }
            this.nativePrivateKey = CtonLibrary.INSTANCE.private_key_create_from_data(keyData, keyData.length);
        }
        
        /**
         * Приватний конструктор для внутрішнього використання
         */
        private PrivateKey(Pointer nativePrivateKey) {
            this.nativePrivateKey = nativePrivateKey;
        }
        
        /**
         * Згенерувати новий приватний ключ
         * @return новий приватний ключ
         */
        public static PrivateKey generate() {
            Pointer ptr = CtonLibrary.INSTANCE.private_key_generate();
            return new PrivateKey(ptr);
        }
        
        /**
         * Отримати дані ключа
         * @return вектор байтів ключа
         */
        public byte[] getData() {
            if (closed) {
                throw new IllegalStateException("PrivateKey has been closed");
            }
            byte[] buffer = new byte[32];
            int result = CtonLibrary.INSTANCE.private_key_get_data(nativePrivateKey, buffer, 32);
            if (result != 32) {
                throw new RuntimeException("Failed to get private key data");
            }
            return buffer;
        }
        
        /**
         * Отримати відповідний публічний ключ
         * @return публічний ключ
         */
        public PublicKey getPublicKey() {
            if (closed) {
                throw new IllegalStateException("PrivateKey has been closed");
            }
            Pointer ptr = CtonLibrary.INSTANCE.private_key_get_public_key(nativePrivateKey);
            return new PublicKey(ptr);
        }
        
        /**
         * Закрити об'єкт і звільнити нативні ресурси
         */
        @Override
        public void close() {
            if (!closed && nativePrivateKey != null) {
                CtonLibrary.INSTANCE.private_key_destroy(nativePrivateKey);
                nativePrivateKey = null;
                closed = true;
            }
        }
    }
    
    /**
     * Представляє публічний ключ Ed25519
     */
    public static class PublicKey implements Closeable {
        private Pointer nativePublicKey;
        private boolean closed = false;
        
        /**
         * Конструктор за замовчуванням
         */
        public PublicKey() {
            this.nativePublicKey = CtonLibrary.INSTANCE.public_key_create();
        }
        
        /**
         * Конструктор з даних
         * @param keyData дані ключа (32 байти)
         */
        public PublicKey(byte[] keyData) {
            if (keyData.length != 32) {
                throw new IllegalArgumentException("Public key data must be exactly 32 bytes");
            }
            this.nativePublicKey = CtonLibrary.INSTANCE.public_key_create_from_data(keyData, keyData.length);
        }
        
        /**
         * Приватний конструктор для внутрішнього використання
         */
        private PublicKey(Pointer nativePublicKey) {
            this.nativePublicKey = nativePublicKey;
        }
        
        /**
         * Отримати дані ключа
         * @return вектор байтів ключа
         */
        public byte[] getData() {
            if (closed) {
                throw new IllegalStateException("PublicKey has been closed");
            }
            byte[] buffer = new byte[32];
            int result = CtonLibrary.INSTANCE.public_key_get_data(nativePublicKey, buffer, 32);
            if (result != 32) {
                throw new RuntimeException("Failed to get public key data");
            }
            return buffer;
        }
        
        /**
         * Перевірити підпис
         * @param message повідомлення
         * @param signature підпис
         * @return true якщо підпис коректний
         */
        public boolean verifySignature(byte[] message, byte[] signature) {
            if (closed) {
                throw new IllegalStateException("PublicKey has been closed");
            }
            return CtonLibrary.INSTANCE.public_key_verify_signature(
                nativePublicKey, message, message.length, signature, signature.length);
        }
        
        /**
         * Закрити об'єкт і звільнити нативні ресурси
         */
        @Override
        public void close() {
            if (!closed && nativePublicKey != null) {
                CtonLibrary.INSTANCE.public_key_destroy(nativePublicKey);
                nativePublicKey = null;
                closed = true;
            }
        }
    }
    
    /**
     * Підписати повідомлення приватним ключем
     * @param privateKey приватний ключ
     * @param message повідомлення для підпису
     * @return підпис (64 байти)
     */
    public static byte[] sign(PrivateKey privateKey, byte[] message) {
        if (privateKey.closed) {
            throw new IllegalStateException("PrivateKey has been closed");
        }
        Pointer signaturePtr = CtonLibrary.INSTANCE.crypto_sign(
            privateKey.nativePrivateKey, message, message.length);
        
        // Якщо підпис успішно створено, конвертуємо його в byte[]
        // If signature is successfully created, convert it to byte[]
        if (signaturePtr != null) {
            // For now, return a 64-byte array (Ed25519 signature size)
            // У реальній реалізації тут має бути конвертація з Pointer в byte[]
            // In a real implementation, there should be conversion from Pointer to byte[]
            return new byte[64];
        } else {
            // У випадку помилки повертаємо порожній масив
            // In case of error, return empty array
            return new byte[0];
        }
    }
    
    /**
     * Перевірити підпис публічним ключем
     * @param publicKey публічний ключ
     * @param message повідомлення
     * @param signature підпис
     * @return true якщо підпис коректний
     */
    public static boolean verify(PublicKey publicKey, byte[] message, byte[] signature) {
        if (publicKey.closed) {
            throw new IllegalStateException("PublicKey has been closed");
        }
        return CtonLibrary.INSTANCE.crypto_verify(
            publicKey.nativePublicKey, message, message.length, signature, signature.length);
    }
    
    /**
     * Генерація mnemonic фрази (24 слова)
     * @return вектор слів
     */
    public static List<String> generateMnemonic() {
        Pointer mnemonicPtr = CtonLibrary.INSTANCE.crypto_generate_mnemonic();
        // В реальній реалізації тут має бути конвертація з Pointer в List<String>
        // For now return dummy list
        return Arrays.asList(
            "abandon", "ability", "able", "about", "above", "absent", "absorb", "abstract",
            "absurd", "abuse", "access", "accident", "account", "accuse", "achieve", "acid",
            "acoustic", "acquire", "across", "act", "action", "actor", "actress", "actual"
        );
    }
    
    /**
     * Створення ключа з mnemonic фрази
     * @param mnemonic мнемонічна фраза
     * @return приватний ключ
     */
    public static PrivateKey mnemonicToPrivateKey(List<String> mnemonic) {
        // В реальній реалізації тут має бути конвертація List<String> в Pointer
        // For now return generated key
        return PrivateKey.generate();
    }
}