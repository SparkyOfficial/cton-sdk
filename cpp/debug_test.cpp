// debug_test.cpp - Simple test to debug OpenSSL functionality
// Author: Андрій Будильников (Sparky)

#include "include/Crypto.h"
#include <iostream>
#include <vector>
#include <string>

using namespace cton;

int main() {
    std::cout << "Debugging OpenSSL functionality..." << std::endl;
    
    try {
        // Test Ed25519
        std::cout << "\n=== Ed25519 Test ===" << std::endl;
        PrivateKey privateKey = PrivateKey::generate();
        std::cout << "Private key generated" << std::endl;
        
        PublicKey publicKey = privateKey.getPublicKey();
        std::cout << "Public key derived" << std::endl;
        
        std::string messageStr = "Hello, OpenSSL!";
        std::vector<uint8_t> message(messageStr.begin(), messageStr.end());
        std::cout << "Message: " << messageStr << std::endl;
        
        std::vector<uint8_t> signature = Crypto::sign(privateKey, message);
        std::cout << "Signature size: " << signature.size() << " bytes" << std::endl;
        
        bool isValid = Crypto::verify(publicKey, message, signature);
        std::cout << "Signature verification: " << (isValid ? "VALID" : "INVALID") << std::endl;
        
        // Test with different message
        std::string differentMessageStr = "Hello, World!";
        std::vector<uint8_t> differentMessage(differentMessageStr.begin(), differentMessageStr.end());
        bool isInvalid = Crypto::verify(publicKey, differentMessage, signature);
        std::cout << "Verification with different message: " << (isInvalid ? "VALID" : "INVALID") << std::endl;
        
        // Test Secp256k1
        std::cout << "\n=== Secp256k1 Test ===" << std::endl;
        Secp256k1PrivateKey secpPrivateKey = Secp256k1PrivateKey::generate();
        std::cout << "Secp256k1 private key generated" << std::endl;
        
        Secp256k1PublicKey secpPublicKey = secpPrivateKey.getPublicKey();
        std::cout << "Secp256k1 public key derived" << std::endl;
        
        std::vector<uint8_t> secpSignature = Crypto::signSecp256k1(secpPrivateKey, message);
        std::cout << "Secp256k1 signature size: " << secpSignature.size() << " bytes" << std::endl;
        
        bool secpIsValid = Crypto::verifySecp256k1(secpPublicKey, message, secpSignature);
        std::cout << "Secp256k1 signature verification: " << (secpIsValid ? "VALID" : "INVALID") << std::endl;
        
        // Test ChaCha20
        std::cout << "\n=== ChaCha20 Test ===" << std::endl;
        std::vector<uint8_t> key(32, 0x01);  // Simple key for testing
        std::vector<uint8_t> nonce(12, 0x02);  // Simple nonce for testing
        std::vector<uint8_t> data = message;  // Use the same message
        
        std::vector<uint8_t> encrypted = ChaCha20::encrypt(data, key, nonce);
        std::cout << "Encrypted data size: " << encrypted.size() << " bytes" << std::endl;
        
        std::vector<uint8_t> decrypted = ChaCha20::decrypt(encrypted, key, nonce);
        std::cout << "Decrypted data size: " << decrypted.size() << " bytes" << std::endl;
        
        bool dataMatches = (data == decrypted);
        std::cout << "Encryption/decryption: " << (dataMatches ? "SUCCESS" : "FAILURE") << std::endl;
        
        std::cout << "\nDebug test completed!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 1;
    }
}