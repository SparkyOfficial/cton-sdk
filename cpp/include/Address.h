// Address.h - робота з адресами TON
// Author: Андрій Будильников (Sparky)
// Адреси в TON можуть бути в різних форматах:
// - raw адреса (0:<hex>)
// - user-friendly адреса (base64url)

#ifndef CTON_ADDRESS_H
#define CTON_ADDRESS_H

#include <string>
#include <vector>
#include <cstdint>

namespace cton {
    
    /**
     * @brief Представляє адресу в мережі TON
     * 
     * TON адреса складається з:
     * - workchain (1 байт)
     * - hash part (32 байти)
     * 
     * Може бути представлена в різних форматах:
     * - raw: -1:<hex_64_chars>
     * - user-friendly: base64url encoded with checksum
     */
    class Address {
    public:
        /**
         * @brief Конструктор за замовчуванням
         */
        Address();
        
        /**
         * @brief Конструктор з raw адреси
         * @param workchain робочий ланцюг
         * @param hashPart хеш-частина адреси (32 байти)
         */
        Address(int8_t workchain, const std::vector<uint8_t>& hashPart);
        
        /**
         * @brief Конструктор з рядка
         * @param addressStr рядок з адресою в будь-якому форматі
         */
        Address(const std::string& addressStr);
        
        /**
         * @brief Отримати робочий ланцюг
         * @return робочий ланцюг
         */
        int8_t getWorkchain() const;
        
        /**
         * @brief Отримати хеш-частину адреси
         * @return хеш-частина (32 байти)
         */
        std::vector<uint8_t> getHashPart() const;
        
        /**
         * @brief Отримати raw-представлення адреси
         * @return рядок з raw адресою
         */
        std::string toRaw() const;
        
        /**
         * @brief Отримати user-friendly представлення адреси
         * @param isBounceable чи адреса bounceable
         * @param isTestOnly чи адреса для тестнету
         * @return рядок з user-friendly адресою
         */
        std::string toUserFriendly(bool isBounceable = true, bool isTestOnly = false) const;
        
        /**
         * @brief Перевірити чи адреса коректна
         * @return true якщо адреса коректна
         */
        bool isValid() const;
        
        /**
         * @brief Оператор порівняння адрес
         * @param other інша адреса
         * @return true якщо адреси рівні
         */
        bool operator==(const Address& other) const;
        
        /**
         * @brief Оператор порівняння адрес
         * @param other інша адреса
         * @return true якщо адреси не рівні
         */
        bool operator!=(const Address& other) const;
        
        /**
         * @brief Створити адресу з raw-формату
         * @param rawAddress raw адреса
         * @return об'єкт Address
         */
        static Address fromRaw(const std::string& rawAddress);
        
        /**
         * @brief Створити адресу з user-friendly формату
         * @param userFriendlyAddress user-friendly адреса
         * @return об'єкт Address
         */
        static Address fromUserFriendly(const std::string& userFriendlyAddress);
        
    private:
        int8_t workchain_;
        std::vector<uint8_t> hashPart_;
        bool valid_;
        
        /**
         * @brief Розпарсити raw адресу
         * @param rawAddress raw адреса
         */
        void parseRaw(const std::string& rawAddress);
        
        /**
         * @brief Розпарсити user-friendly адресу
         * @param userFriendlyAddress user-friendly адреса
         */
        void parseUserFriendly(const std::string& userFriendlyAddress);
    };
}

#endif // CTON_ADDRESS_H