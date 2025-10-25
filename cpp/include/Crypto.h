// Crypto.h - криптографічні функції для TON
// Author: Андрій Будильников (Sparky)
// Криптографічні функції Ed25519 для TON
// Ed25519 cryptographic functions for TON
// Криптографические функции Ed25519 для TON

#ifndef CTON_CRYPTO_H
#define CTON_CRYPTO_H

#include <vector>
#include <string>
#include <memory>

namespace cton {
    
    // Forward declaration
    class Mnemonic;
    
    class PrivateKey {
    public:
        /**
         * Конструктор за замовчуванням
         */
        PrivateKey();
        
        /**
         * Конструктор з даних ключа
         * @param keyData дані ключа (32 байти)
         */
        explicit PrivateKey(const std::vector<uint8_t>& keyData);
        
        /**
         * Генерація нового приватного ключа
         * @return новий приватний ключ
         */
        static PrivateKey generate();
        
        /**
         * Отримати дані ключа
         * @return вектор байтів ключа
         */
        std::vector<uint8_t> getData() const;
        
        /**
         * Отримати відповідний публічний ключ
         * @return публічний ключ
         */
        class PublicKey getPublicKey() const;
        
    private:
        std::vector<uint8_t> keyData_;
    };
    
    class PublicKey {
    public:
        /**
         * Конструктор за замовчуванням
         */
        PublicKey();
        
        /**
         * Конструктор з даних ключа
         * @param keyData дані ключа (32 байти)
         */
        explicit PublicKey(const std::vector<uint8_t>& keyData);
        
        /**
         * Отримати дані ключа
         * @return вектор байтів ключа
         */
        std::vector<uint8_t> getData() const;
        
        /**
         * Перевірити підпис
         * @param message повідомлення
         * @param signature підпис
         * @return true якщо підпис коректний
         */
        bool verifySignature(const std::vector<uint8_t>& message, 
                           const std::vector<uint8_t>& signature) const;
        
    private:
        std::vector<uint8_t> keyData_;
    };
    
    class Crypto {
    public:
        /**
         * Підписати повідомлення приватним ключем
         * @param privateKey приватний ключ
         * @param message повідомлення для підпису
         * @return підпис (64 байти)
         */
        static std::vector<uint8_t> sign(const PrivateKey& privateKey, 
                                       const std::vector<uint8_t>& message);
        
        /**
         * Перевірити підпис публічним ключем
         * @param publicKey публічний ключ
         * @param message повідомлення
         * @param signature підпис
         * @return true якщо підпис коректний
         */
        static bool verify(const PublicKey& publicKey,
                         const std::vector<uint8_t>& message,
                         const std::vector<uint8_t>& signature);
        
        /**
         * Генерація mnemonic фрази (24 слова)
         * @return вектор слів
         */
        static std::vector<std::string> generateMnemonic();
        
        /**
         * Створення ключа з mnemonic фрази
         * @param mnemonic мнемонічна фраза
         * @return приватний ключ
         */
        static PrivateKey mnemonicToPrivateKey(const std::vector<std::string>& mnemonic);
    };
    
}

#endif // CTON_CRYPTO_H