// main.cpp - тестування native interface
// Author: Андрій Будильников (Sparky)
// Testing native interface
// Тестирование native интерфейса

#include "../include/NativeInterface.h"
#include <iostream>
#include <vector>

int main() {
    std::cout << "Testing CTON-SDK Native Interface\n";
    
    // Test cell creation
    void* cell = cell_create();
    if (cell) {
        std::cout << "Cell created successfully\n";
        
        // Test storing data
        if (cell_store_uint(cell, 32, 0x12345678)) {
            std::cout << "Stored uint successfully\n";
        } else {
            std::cout << "Failed to store uint\n";
        }
        
        // Test getting bit size
        int bitSize = cell_get_bit_size(cell);
        if (bitSize >= 0) {
            std::cout << "Cell bit size: " << bitSize << "\n";
        } else {
            std::cout << "Failed to get cell bit size\n";
        }
        
        // Clean up
        cell_destroy(cell);
        std::cout << "Cell destroyed\n";
    } else {
        std::cout << "Failed to create cell\n";
    }
    
    // Test address creation
    void* address = address_create();
    if (address) {
        std::cout << "Address created successfully\n";
        address_destroy(address);
        std::cout << "Address destroyed\n";
    } else {
        std::cout << "Failed to create address\n";
    }
    
    // Test private key creation
    void* privateKey = private_key_create();
    if (privateKey) {
        std::cout << "Private key created successfully\n";
        private_key_destroy(privateKey);
        std::cout << "Private key destroyed\n";
    } else {
        std::cout << "Failed to create private key\n";
    }
    
    // Test public key creation
    void* publicKey = public_key_create();
    if (publicKey) {
        std::cout << "Public key created successfully\n";
        public_key_destroy(publicKey);
        std::cout << "Public key destroyed\n";
    } else {
        std::cout << "Failed to create public key\n";
    }
    
    // Test BOC creation
    void* boc = boc_create();
    if (boc) {
        std::cout << "BOC created successfully\n";
        boc_destroy(boc);
        std::cout << "BOC destroyed\n";
    } else {
        std::cout << "Failed to create BOC\n";
    }
    
    std::cout << "All tests completed!\n";
    return 0;
}