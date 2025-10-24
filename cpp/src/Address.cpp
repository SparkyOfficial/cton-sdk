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
        
        // Поки що повертаємо порожній рядок, реалізація base64url буде пізніше
        // TODO: реалізувати base64url кодування з CRC
        return "";
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
        // TODO: реалізувати парсинг user-friendly адрес
        valid_ = false;
    }
}