// Address.cpp - реалізація роботи з адресами TON
// Author: Андрій Будильников (Sparky)

#include "../include/Address.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstring>

// Для base64 кодування
static const std::string base64_chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

// Для base64url кодування (використовується в TON)
static const std::string base64url_chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789-_";

namespace cton {
    
    Address::Address() : workchain_(0), valid_(false) {
        hashPart_.resize(32, 0);
    }
    
    Address::Address(int8_t workchain, const std::vector<uint8_t>& hashPart) 
        : workchain_(workchain), hashPart_(hashPart), valid_(true) {
        if (hashPart_.size() != 32) {
            throw std::invalid_argument("Hash part must be exactly 32 bytes");
        }
    }
    
    Address::Address(const std::string& addressStr) : valid_(false) {
        hashPart_.resize(32, 0);
        
        if (addressStr.empty()) {
            return;
        }
        
        // Визначення типу адреси
        if (addressStr.find(':') != std::string::npos) {
            // Raw адреса
            parseRaw(addressStr);
        } else if (addressStr.length() == 48) {
            // User-friendly адреса
            parseUserFriendly(addressStr);
        }
    }
    
    int8_t Address::getWorkchain() const {
        return workchain_;
    }
    
    void Address::setWorkchain(int8_t workchain) {
        workchain_ = workchain;
    }

    std::vector<uint8_t> Address::getHashPart() const {
        return hashPart_;
    }
    
    void Address::setHashPart(const std::vector<uint8_t>& hashPart) {
        if (hashPart.size() != 32) {
            throw std::invalid_argument("Hash part must be exactly 32 bytes");
        }
        hashPart_ = hashPart;
        valid_ = true;
    }
    
    std::string Address::toRaw() const {
        if (!valid_) {
            return "";
        }
        
        std::stringstream ss;
        ss << static_cast<int>(workchain_) << ":";
        
        ss << std::hex << std::setfill('0');
        for (const auto& byte : hashPart_) {
            ss << std::setw(2) << static_cast<int>(byte);
        }
        
        return ss.str();
    }
    
    std::string Address::toUserFriendly(bool isBounceable, bool isTestOnly) const {
        if (!valid_) {
            return "";
        }
        
        // Реалізація base64url кодування з CRC
        // Implementation of base64url encoding with CRC
        
        // Створюємо буфер для кодування
        std::vector<uint8_t> buffer;
        buffer.reserve(36); // 1 байт tag + 32 байти hash + 3 байти CRC
        
        // Додаємо tag байт
        uint8_t tag = 0x11; // Base tag for address
        if (!isBounceable) {
            tag |= 0x80; // Set non-bounceable bit
        }
        if (isTestOnly) {
            tag |= 0x40; // Set testnet bit
        }
        if (workchain_ != -1) {
            tag |= 0x20; // Set workchain bit if not -1
        }
        buffer.push_back(tag);
        
        // Додаємо workchain (1 байт)
        buffer.push_back(static_cast<uint8_t>(workchain_));
        
        // Додаємо hash part (32 байти)
        buffer.insert(buffer.end(), hashPart_.begin(), hashPart_.end());
        
        // Обчислюємо CRC16 (спрощена реалізація)
        // Calculate CRC16 (simplified implementation)
        uint16_t crc = 0;
        for (size_t i = 0; i < buffer.size(); ++i) {
            crc ^= (uint16_t)buffer[i] << 8;
            for (int j = 0; j < 8; ++j) {
                if (crc & 0x8000) {
                    crc = (crc << 1) ^ 0x1021; // CRC-16-CCITT polynomial
                } else {
                    crc <<= 1;
                }
            }
            crc &= 0xFFFF;
        }
        
        // Додаємо CRC до буфера
        // Add CRC to buffer
        buffer.push_back((crc >> 8) & 0xFF);
        buffer.push_back(crc & 0xFF);
        
        // Кодуємо в base64url
        // Encode to base64url
        std::string result;
        result.reserve((buffer.size() * 4 + 2) / 3);
        
        for (size_t i = 0; i < buffer.size(); i += 3) {
            uint32_t triple = (buffer[i] << 16) | 
                         ((i + 1 < buffer.size() ? buffer[i + 1] : 0) << 8) | 
                         (i + 2 < buffer.size() ? buffer[i + 2] : 0);
        
            result.push_back(base64url_chars[(triple >> 18) & 0x3F]);
            result.push_back(base64url_chars[(triple >> 12) & 0x3F]);
            if (i + 1 < buffer.size()) {
                result.push_back(base64url_chars[(triple >> 6) & 0x3F]);
            } else {
                break;
            }
            if (i + 2 < buffer.size()) {
                result.push_back(base64url_chars[triple & 0x3F]);
            } else {
                break;
            }
        }
        
        return result;
    }
    
    bool Address::isValid() const {
        return valid_;
    }
    
    bool Address::operator==(const Address& other) const {
        return workchain_ == other.workchain_ && 
               hashPart_ == other.hashPart_ && 
               valid_ == other.valid_;
    }
    
    bool Address::operator!=(const Address& other) const {
        return !(*this == other);
    }
    
    Address Address::fromRaw(const std::string& rawAddress) {
        Address addr;
        addr.parseRaw(rawAddress);
        return addr;
    }
    
    Address Address::fromUserFriendly(const std::string& userFriendlyAddress) {
        Address addr;
        addr.parseUserFriendly(userFriendlyAddress);
        return addr;
    }
    
    void Address::parseRaw(const std::string& rawAddress) {
        valid_ = false;
        
        size_t colonPos = rawAddress.find(':');
        if (colonPos == std::string::npos || colonPos == 0 || colonPos >= rawAddress.length() - 1) {
            return;
        }
        
        // Парсинг workchain
        std::string workchainStr = rawAddress.substr(0, colonPos);
        try {
            workchain_ = static_cast<int8_t>(std::stoi(workchainStr));
        } catch (const std::exception&) {
            return;
        }
        
        // Парсинг hash part
        std::string hashStr = rawAddress.substr(colonPos + 1);
        if (hashStr.length() != 64) { // 32 байти * 2 символи на байт
            return;
        }
        
        hashPart_.clear();
        for (size_t i = 0; i < 64; i += 2) {
            std::string byteStr = hashStr.substr(i, 2);
            try {
                uint8_t byte = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));
                hashPart_.push_back(byte);
            } catch (const std::exception&) {
                hashPart_.clear();
                hashPart_.resize(32, 0);
                return;
            }
        }
        
        valid_ = true;
    }
    
    void Address::parseUserFriendly(const std::string& userFriendlyAddress) {
        // Реалізація парсингу user-friendly адрес
        // Implementation of parsing user-friendly address
        
        // Декодуємо з base64url
        // Decode from base64url
        std::vector<uint8_t> buffer;
        buffer.reserve(userFriendlyAddress.length() * 3 / 4);
        
        uint32_t accumulator = 0;
        int bitCount = 0;
        
        for (char c : userFriendlyAddress) {
            size_t pos = base64url_chars.find(c);
            if (pos == std::string::npos) {
                valid_ = false;
                return;
            }
        
            accumulator = (accumulator << 6) | pos;
            bitCount += 6;
        
            if (bitCount >= 8) {
                bitCount -= 8;
                buffer.push_back((accumulator >> bitCount) & 0xFF);
            }
        }
        
        // Перевіряємо мінімальний розмір (tag + workchain + hash + crc)
        // Check minimum size (tag + workchain + hash + crc)
        if (buffer.size() < 36) {
            valid_ = false;
            return;
        }
        
        // Перевіряємо CRC
        // Check CRC
        uint16_t receivedCrc = (buffer[buffer.size() - 2] << 8) | buffer[buffer.size() - 1];
        uint16_t calculatedCrc = 0;
        
        // Обчислюємо CRC для всіх байтів окрім останніх двох
        // Calculate CRC for all bytes except the last two
        for (size_t i = 0; i < buffer.size() - 2; ++i) {
            calculatedCrc ^= (uint16_t)buffer[i] << 8;
            for (int j = 0; j < 8; ++j) {
                if (calculatedCrc & 0x8000) {
                    calculatedCrc = (calculatedCrc << 1) ^ 0x1021; // CRC-16-CCITT polynomial
                } else {
                    calculatedCrc <<= 1;
                }
            }
            calculatedCrc &= 0xFFFF;
        }
        
        if (receivedCrc != calculatedCrc) {
            valid_ = false;
            return;
        }
        
        // Парсимо tag байт
        // Parse tag byte
        uint8_t tag = buffer[0];
        bool isBounceable = (tag & 0x80) == 0;
        bool isTestOnly = (tag & 0x40) != 0;
        bool hasWorkchain = (tag & 0x20) != 0;
        
        // Отримуємо workchain
        // Get workchain
        workchain_ = static_cast<int8_t>(buffer[1]);
        
        // Отримуємо hash part (32 байти)
        // Get hash part (32 bytes)
        hashPart_.clear();
        hashPart_.insert(hashPart_.end(), buffer.begin() + 2, buffer.begin() + 34);
        
        valid_ = true;
    }

}