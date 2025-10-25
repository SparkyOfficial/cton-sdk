// BenchmarkTest.cpp - тестування продуктивності та бенчмарки
// Author: Андрій Будильников (Sparky)
// Performance testing and benchmarks
// Тестирование производительности и бенчмарки

#include "../include/Boc.h"
#include "../include/Cell.h"
#include "../include/Crypto.h"
#include "../include/Address.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <memory>

using namespace cton;

class Benchmark {
private:
    std::string name_;
    std::chrono::high_resolution_clock::time_point start_;
    
public:
    Benchmark(const std::string& name) : name_(name) {
        start_ = std::chrono::high_resolution_clock::now();
    }
    
    ~Benchmark() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
        std::cout << "   " << name_ << ": " << duration.count() << " microseconds" << std::endl;
    }
};

void testCellPerformance() {
    std::cout << "=== Cell Performance Tests ===" << std::endl;
    
    {
        Benchmark b("Create 10000 cells");
        for (int i = 0; i < 10000; ++i) {
            Cell cell;
            cell.storeUInt(32, i);
        }
    }
    
    {
        Benchmark b("CellBuilder with 1000 operations");
        CellBuilder builder;
        for (int i = 0; i < 1000; ++i) {
            builder.storeUInt(32, i);
        }
        auto cell = builder.build();
    }
}

void testCryptoPerformance() {
    std::cout << "=== Crypto Performance Tests ===" << std::endl;
    
    {
        Benchmark b("Generate 1000 private keys");
        for (int i = 0; i < 1000; ++i) {
            auto key = PrivateKey::generate();
        }
    }
    
    {
        Benchmark b("Generate and sign 100 messages");
        auto privateKey = PrivateKey::generate();
        auto publicKey = privateKey.getPublicKey();
        std::vector<uint8_t> message = {0x01, 0x02, 0x03, 0x04, 0x05};
        
        for (int i = 0; i < 100; ++i) {
            auto signature = Crypto::sign(privateKey, message);
            bool verified = Crypto::verify(publicKey, message, signature);
        }
    }
}

void testAddressPerformance() {
    std::cout << "=== Address Performance Tests ===" << std::endl;
    
    {
        Benchmark b("Parse 1000 addresses");
        std::string addressStr = "0:1234567890123456789012345678901234567890123456789012345678901234";
        for (int i = 0; i < 1000; ++i) {
            Address addr(addressStr);
        }
    }
    
    {
        Benchmark b("Convert 1000 addresses to user-friendly format");
        Address addr("0:1234567890123456789012345678901234567890123456789012345678901234");
        for (int i = 0; i < 1000; ++i) {
            std::string uf = addr.toUserFriendly(true, false);
        }
    }
}

void testBocPerformance() {
    std::cout << "=== BOC Performance Tests ===" << std::endl;
    
    {
        Benchmark b("Create BOC with 1000 nested cells");
        auto root = std::make_shared<Cell>();
        root->storeUInt(32, 0x12345678);
        
        auto current = root;
        for (int i = 0; i < 1000; ++i) {
            auto newCell = std::make_shared<Cell>();
            newCell->storeUInt(32, i);
            current->addReference(newCell);
            current = newCell;
        }
        
        Boc boc(root);
    }
    
    {
        Benchmark b("Serialize BOC with 1000 cells");
        auto root = std::make_shared<Cell>();
        root->storeUInt(32, 0x12345678);
        
        auto current = root;
        for (int i = 0; i < 1000; ++i) {
            auto newCell = std::make_shared<Cell>();
            newCell->storeUInt(32, i);
            current->addReference(newCell);
            current = newCell;
        }
        
        Boc boc(root);
        auto serialized = boc.serialize(true, true);
    }
    
    {
        Benchmark b("Deserialize BOC with 1000 cells");
        auto root = std::make_shared<Cell>();
        root->storeUInt(32, 0x12345678);
        
        auto current = root;
        for (int i = 0; i < 1000; ++i) {
            auto newCell = std::make_shared<Cell>();
            newCell->storeUInt(32, i);
            current->addReference(newCell);
            current = newCell;
        }
        
        Boc boc(root);
        auto serialized = boc.serialize(true, true);
        auto deserialized = Boc::deserialize(serialized);
    }
}

int main() {
    std::cout << "Running CTON-SDK Performance Benchmarks" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    try {
        testCellPerformance();
        std::cout << std::endl;
        
        testCryptoPerformance();
        std::cout << std::endl;
        
        testAddressPerformance();
        std::cout << std::endl;
        
        testBocPerformance();
        std::cout << std::endl;
        
        std::cout << "All performance benchmarks completed successfully!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in performance benchmarks: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error in performance benchmarks" << std::endl;
        return 1;
    }
}