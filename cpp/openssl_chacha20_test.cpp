// openssl_chacha20_test.cpp - Simple test for OpenSSL ChaCha20 functionality
// Author: Андрій Будильников (Sparky)

#include <openssl/evp.h>
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::cout << "Testing OpenSSL ChaCha20 functionality..." << std::endl;
    
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
        
        // Encrypt
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            std::cout << "Failed to create encryption context" << std::endl;
            return 1;
        }
        
        // Initialize encryption
        if (EVP_EncryptInit_ex(ctx, EVP_chacha20(), nullptr, key, nonce) != 1) {
            std::cout << "Failed to initialize encryption" << std::endl;
            EVP_CIPHER_CTX_free(ctx);
            return 1;
        }
        
        // Encrypt data
        std::vector<uint8_t> ciphertext(message.size());
        int len;
        
        if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, message.data(), message.size()) != 1) {
            std::cout << "Failed to encrypt data" << std::endl;
            EVP_CIPHER_CTX_free(ctx);
            return 1;
        }
        
        EVP_CIPHER_CTX_free(ctx);
        
        std::cout << "Encrypted size: " << len << " bytes" << std::endl;
        std::cout << "Encrypted data: ";
        for (size_t i = 0; i < ciphertext.size(); ++i) {
            std::cout << (int)ciphertext[i] << " ";
        }
        std::cout << std::endl;
        
        // Decrypt
        EVP_CIPHER_CTX* decrypt_ctx = EVP_CIPHER_CTX_new();
        if (!decrypt_ctx) {
            std::cout << "Failed to create decryption context" << std::endl;
            return 1;
        }
        
        // Initialize decryption
        if (EVP_EncryptInit_ex(decrypt_ctx, EVP_chacha20(), nullptr, key, nonce) != 1) {
            std::cout << "Failed to initialize decryption" << std::endl;
            EVP_CIPHER_CTX_free(decrypt_ctx);
            return 1;
        }
        
        // Decrypt data
        std::vector<uint8_t> decrypted(ciphertext.size());
        int decrypt_len;
        
        if (EVP_EncryptUpdate(decrypt_ctx, decrypted.data(), &decrypt_len, ciphertext.data(), ciphertext.size()) != 1) {
            std::cout << "Failed to decrypt data" << std::endl;
            EVP_CIPHER_CTX_free(decrypt_ctx);
            return 1;
        }
        
        EVP_CIPHER_CTX_free(decrypt_ctx);
        
        std::cout << "Decrypted size: " << decrypt_len << " bytes" << std::endl;
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