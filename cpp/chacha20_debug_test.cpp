// chacha20_debug_test.cpp - Debug test for ChaCha20 functionality
// Author: Андрій Будильников (Sparky)

#include "include/Crypto.h"
#include <iostream>
#include <vector>
#include <string>

using namespace cton;

int main() {
    std::cout << "Debug testing ChaCha20 functionality..." << std::endl;
    
    try {
        // Test data
        std::string messageStr = "This is a secret wallet data!";
        std::vector<uint8_t> message(messageStr.begin(), messageStr.end());
        std::cout << "Message: " << messageStr << std::endl;
        std::cout << "Message size: " << message.size() << " bytes" << std::endl;
        std::cout << "Message data: ";
        for (size_t i = 0; i < message.size(); ++i) {
            std::cout << (int)message[i] << " ";
        }
        std::cout << std::endl;
        
        // Key and nonce
        std::vector<uint8_t> key(32, 0x12);
        std::vector<uint8_t> nonce(12, 0x34);
        
        std::cout << "Key: ";
        for (size_t i = 0; i < key.size(); ++i) {
            std::cout << (int)key[i] << " ";
        }
        std::cout << std::endl;
        
        std::cout << "Nonce: ";
        for (size_t i = 0; i < nonce.size(); ++i) {
            std::cout << (int)nonce[i] << " ";
        }
        std::cout << std::endl;
        
        // Encrypt using the C++ ChaCha20 class directly
        std::vector<uint8_t> encrypted = ChaCha20::encrypt(message, key, nonce);
        std::cout << "Encrypted size: " << encrypted.size() << " bytes" << std::endl;
        std::cout << "Encrypted data: ";
        for (size_t i = 0; i < encrypted.size(); ++i) {
            std::cout << (int)encrypted[i] << " ";
        }
        std::cout << std::endl;
        
        // Decrypt using the C++ ChaCha20 class directly
        std::vector<uint8_t> decrypted = ChaCha20::decrypt(encrypted, key, nonce);
        std::cout << "Decrypted size: " << decrypted.size() << " bytes" << std::endl;
        std::cout << "Decrypted data: ";
        for (size_t i = 0; i < decrypted.size(); ++i) {
            std::cout << (int)decrypted[i] << " ";
        }
        std::cout << std::endl;
        
        std::string decryptedStr(decrypted.begin(), decrypted.end());
        std::cout << "Decrypted message: " << decryptedStr << std::endl;
        
        // Check if encryption/decryption worked
        bool success = (message == decrypted);
        std::cout << "Encryption/decryption: " << (success ? "SUCCESS" : "FAILURE") << std::endl;
        
        return success ? 0 : 1;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 1;
    }
}