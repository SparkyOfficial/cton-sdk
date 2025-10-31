// chacha20_double_encrypt_test.cpp - Test double encryption with ChaCha20
// Author: Андрій Будильников (Sparky)

#include "include/Crypto.h"
#include <iostream>
#include <vector>
#include <string>

using namespace cton;

int main() {
    std::cout << "Testing double encryption with ChaCha20..." << std::endl;
    
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
        
        // First encryption
        std::vector<uint8_t> encrypted1 = ChaCha20::encrypt(message, key, nonce);
        std::cout << "First encryption result: ";
        for (size_t i = 0; i < encrypted1.size(); ++i) {
            std::cout << (int)encrypted1[i] << " ";
        }
        std::cout << std::endl;
        
        // Second encryption with same key and nonce
        std::vector<uint8_t> encrypted2 = ChaCha20::encrypt(encrypted1, key, nonce);
        std::cout << "Second encryption result: ";
        for (size_t i = 0; i < encrypted2.size(); ++i) {
            std::cout << (int)encrypted2[i] << " ";
        }
        std::cout << std::endl;
        
        // Check if second encryption gives us back the original message
        bool success = (message == encrypted2);
        std::cout << "Double encryption test: " << (success ? "SUCCESS" : "FAILURE") << std::endl;
        
        if (success) {
            std::string decryptedStr(encrypted2.begin(), encrypted2.end());
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