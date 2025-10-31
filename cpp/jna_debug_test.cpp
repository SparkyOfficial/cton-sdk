// jna_debug_test.cpp - Test to debug JNA ChaCha20 issue
// Author: Андрій Будильников (Sparky)

#include "include/NativeInterface.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::cout << "Testing JNA ChaCha20 functionality..." << std::endl;
    
    try {
        // Test data (same as Java test)
        std::string messageStr = "This is a secret wallet data!";
        std::vector<uint8_t> message(messageStr.begin(), messageStr.end());
        std::cout << "Message: " << messageStr << std::endl;
        std::cout << "Message size: " << message.size() << " bytes" << std::endl;
        std::cout << "Message data: ";
        for (size_t i = 0; i < message.size(); ++i) {
            std::cout << (int)message[i] << " ";
        }
        std::cout << std::endl;
        
        // Key and nonce (same as Java test)
        uint8_t key[32];
        for (int i = 0; i < 32; i++) {
            key[i] = 0x12;
        }
        uint8_t nonce[12];
        for (int i = 0; i < 12; i++) {
            nonce[i] = 0x34;
        }
        
        std::cout << "Key: ";
        for (int i = 0; i < 32; i++) {
            std::cout << (int)key[i] << " ";
        }
        std::cout << std::endl;
        
        std::cout << "Nonce: ";
        for (int i = 0; i < 12; i++) {
            std::cout << (int)nonce[i] << " ";
        }
        std::cout << std::endl;
        
        // Call the native function directly (as JNA would)
        void* resultPtr = crypto_chacha20_encrypt(message.data(), message.size(), key, nonce);
        
        if (resultPtr == nullptr) {
            std::cout << "Encryption failed!" << std::endl;
            return 1;
        }
        
        // Get the result data
        uint8_t* result = static_cast<uint8_t*>(resultPtr);
        std::vector<uint8_t> encrypted(message.size());
        for (size_t i = 0; i < message.size(); ++i) {
            encrypted[i] = result[i];
        }
        
        std::cout << "Encrypted size: " << encrypted.size() << " bytes" << std::endl;
        std::cout << "Encrypted data: ";
        for (size_t i = 0; i < encrypted.size(); ++i) {
            std::cout << (int)encrypted[i] << " ";
        }
        std::cout << std::endl;
        
        // Free the memory
        free(resultPtr);
        
        // Now decrypt
        void* decryptResultPtr = crypto_chacha20_encrypt(encrypted.data(), encrypted.size(), key, nonce);
        
        if (decryptResultPtr == nullptr) {
            std::cout << "Decryption failed!" << std::endl;
            return 1;
        }
        
        // Get the decrypted data
        uint8_t* decryptResult = static_cast<uint8_t*>(decryptResultPtr);
        std::vector<uint8_t> decrypted(encrypted.size());
        for (size_t i = 0; i < encrypted.size(); ++i) {
            decrypted[i] = decryptResult[i];
        }
        
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
        
        // Free the memory
        free(decryptResultPtr);
        
        return success ? 0 : 1;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 1;
    }
}