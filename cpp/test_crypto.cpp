#include "include/Crypto.h"
#include <iostream>

int main() {
    // Test that we can create a PrivateKey
    cton::PrivateKey key = cton::PrivateKey::generate();
    
    // Test that we can get data from it
    std::vector<uint8_t> data = key.getData();
    
    std::cout << "PrivateKey data size: " << data.size() << std::endl;
    
    return 0;
}