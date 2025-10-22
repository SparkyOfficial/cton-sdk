// Crypto.h - криптографічні функції для TON
// Author: Андрій Будильников (Sparky)
// Реалізація криптографічних функцій Ed25519 для TON

#ifndef CTON_CRYPTO_H
#define CTON_CRYPTO_H

#include <vector>
#include <string>
#include <cstdint>
#include <memory>

namespace cton {
    
    /**
     * @brief Представляє приватний ключ Ed25519
     */
    class PrivateKey {
    public:
        /**
         * @brief Конструктор за замовчуванням
         */
        PrivateKey();
        
        /**
         * @brief Конструктор з вектора байтів
         * @param keyData дані ключа (32 байти)
         */
        PrivateKey(const std::vector<uint8_t>& keyData);
        
        /**
         * @brief Згенерувати новий приватний ключ
         * @return новий приватний ключ
         */
        static PrivateKey generate();
        
        /**
         * @brief Отримати дані ключа
         * @return вектор байтів ключа
         */
        std::vector<uint8_t> getData() const;
        
        /**
         * @brief Отримати відповідний публічний ключ
         * @return публічний ключ
         */
        class PublicKey getPublicKey() const;
        
    private:
        std::vector<uint8_t> keyData_;
    };
    
    /**
     * @brief Представляє публічний ключ Ed25519
     */
    class PublicKey {
    public:
        /**
         * @brief Конструктор за замовчуванням
         */
        PublicKey();
        
        /**
         * @brief Конструктор з вектора байтів
         * @param keyData дані ключа (32 байти)
         */
        PublicKey(const std::vector<uint8_t>& keyData);
        
        /**
         * @brief Отримати дані ключа
         * @return вектор байтів ключа
         */
        std::vector<uint8_t> getData() const;
        
        /**
         * @brief Перевірити підпис
         * @param message повідомлення
         * @param signature підпис
         * @return true якщо підпис коректний
         */
        bool verifySignature(const std::vector<uint8_t>& message, 
                           const std::vector<uint8_t>& signature) const;
        
    private:
        std::vector<uint8_t> keyData_;
    };
    
    /**
     * @brief Статичні криптографічні функції
     */
    class Crypto {
    public:
        /**
         * @brief Підписати повідомлення приватним ключем
         * @param privateKey приватний ключ
         * @param message повідомлення для підпису
         * @return підпис (64 байти)
         */
        static std::vector<uint8_t> sign(const PrivateKey& privateKey, 
                                       const std::vector<uint8_t>& message);
        
        /**
         * @brief Перевірити підпис публічним ключем
         * @param publicKey публічний ключ
         * @param message повідомлення
         * @param signature підпис
         * @return true якщо підпис коректний
         */
        static bool verify(const PublicKey& publicKey,
                         const std::vector<uint8_t>& message,
                         const std::vector<uint8_t>& signature);
        
        /**
         * @brief Генерація mnemonic фрази (24 слова)
         * @return вектор слів
         */
        static std::vector<std::string> generateMnemonic();
        
        /**
         * @brief Створення ключа з mnemonic фрази
         * @param mnemonic мнемонічна фраза
         * @return приватний ключ
         */
        static PrivateKey mnemonicToPrivateKey(const std::vector<std::string>& mnemonic);
    };
}

#endif // CTON_CRYPTO_H