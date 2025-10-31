// Simple test to verify SHA256 linking
// Author: Андрій Будильников (Sparky)

#include <openssl/sha.h>
#include <iostream>
#include <vector>

int main() {
    // Test data
    std::vector<unsigned char> data = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'};
    
    // Hash buffer
    unsigned char hash[SHA256_DIGEST_LENGTH];
    
    // Calculate SHA256
    SHA256(data.data(), data.size(), hash);
    
    // Print result
    std::cout << "SHA256 hash: ";
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        printf("%02x", hash[i]);
    }
    std::cout << std::endl;
    
    return 0;
}