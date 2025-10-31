// Simple test to verify OpenSSL linking
// Author: Андрій Будильников (Sparky)

#include "../openssl-3.6.0/include/openssl/sha.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

int main() {
    // Test data
    std::vector<unsigned char> data = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'};
    
    // Hash buffer
    unsigned char hash[32]; // SHA256_DIGEST_LENGTH
    
    // Calculate SHA256
    SHA256(data.data(), data.size(), hash);
    
    // Print result
    std::cout << "SHA256 hash: ";
    for (int i = 0; i < 32; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    std::cout << std::endl;
    
    // Expected hash for "Hello World"
    std::string expected = "a591a6d40bf420404a011733cfb7b190d62c65bf0bcda32b57b277d9ad9f146e";
    
    // Convert hash to string for comparison
    std::stringstream ss;
    for (int i = 0; i < 32; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    
    if (ss.str() == expected) {
        std::cout << "SHA256 test PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "SHA256 test FAILED" << std::endl;
        std::cout << "Expected: " << expected << std::endl;
        std::cout << "Got:      " << ss.str() << std::endl;
        return 1;
    }
}