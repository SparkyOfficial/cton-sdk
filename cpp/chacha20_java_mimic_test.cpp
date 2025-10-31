// chacha20_java_mimic_test.cpp - Test that mimics exactly what Java code does
// Author: Андрій Будильников (Sparky)

#include "include/NativeInterface.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::cout << "Testing ChaCha20 functionality mimicking Java code exactly..." << std::endl;
    
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
        
        // Call the native function exactly as Java would (first encryption)
        void* resultPtr1 = crypto_chacha20_encrypt(message.data(), static_cast<int>(message.size()), key, nonce);
        
        if (resultPtr1 == nullptr) {
            std::cout << "First encryption failed!" << std::endl;
            return 1;
        }
        
        // Get the result data
        uint8_t* result1 = static_cast<uint8_t*>(resultPtr1);
        std::vector<uint8_t> encrypted(message.size());
        for (size_t i = 0; i < message.size(); ++i) {
            encrypted[i] = result1[i];
        }
        
        std::cout << "First encryption result: ";
        for (size_t i = 0; i < encrypted.size(); ++i) {
            std::cout << (int)encrypted[i] << " ";
        }
        std::cout << std::endl;
        
        // Free the memory
        free(resultPtr1);
        
        // Call the native function exactly as Java would (second encryption for decryption)
        void* resultPtr2 = crypto_chacha20_encrypt(encrypted.data(), static_cast<int>(encrypted.size()), key, nonce);
        
        if (resultPtr2 == nullptr) {
            std::cout << "Second encryption failed!" << std::endl;
            return 1;
        }
        
        // Get the result data
        uint8_t* result2 = static_cast<uint8_t*>(resultPtr2);
        std::vector<uint8_t> decrypted(encrypted.size());
        for (size_t i = 0; i < encrypted.size(); ++i) {
            decrypted[i] = result2[i];
        }
        
        std::cout << "Second encryption result: ";
        for (size_t i = 0; i < decrypted.size(); ++i) {
            std::cout << (int)decrypted[i] << " ";
        }
        std::cout << std::endl;
        
        // Free the memory
        free(resultPtr2);
        
        // Check if second encryption gives us back the original message
        bool success = (message == decrypted);
        std::cout << "Double encryption test: " << (success ? "SUCCESS" : "FAILURE") << std::endl;
        
        if (success) {
            std::string decryptedStr(decrypted.begin(), decrypted.end());
            std::cout << "Decrypted message: " << decryptedStr << std::endl;
        }
        
        return success ? 0 : 1;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 1;
    }
}