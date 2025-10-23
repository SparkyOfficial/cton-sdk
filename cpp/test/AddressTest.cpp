// AddressTest.cpp - тести для Address класу
// Author: Андрій Будильников (Sparky)
// Unit tests for Address class
// Модульные тесты для класса Address

#include "TestFramework.h"
#include "../include/Address.h"
#include <cstring>

using namespace cton;

TEST(AddressCreation) {
    Address addr;
    ASSERT_FALSE(addr.isValid());
    
    // Test workchain
    ASSERT_EQUAL(0, addr.getWorkchain());
    
    // Test hash part
    auto hashPart = addr.getHashPart();
    ASSERT_EQUAL(32, hashPart.size());
}

TEST(AddressFromRaw) {
    // Test valid raw address
    Address addr("0:1234567890123456789012345678901234567890123456789012345678901234");
    ASSERT_TRUE(addr.isValid());
    ASSERT_EQUAL(0, addr.getWorkchain());
    
    auto hashPart = addr.getHashPart();
    ASSERT_EQUAL(32, hashPart.size());
}

TEST(AddressFromRawInvalid) {
    // Test invalid raw address
    Address addr("invalid_address");
    ASSERT_FALSE(addr.isValid());
}

TEST(AddressFromRawWrongLength) {
    // Test raw address with wrong hash length
    Address addr("0:12345678901234567890123456789012345678901234567890123456789012"); // Missing one char
    ASSERT_FALSE(addr.isValid());
}

TEST(AddressToRaw) {
    Address addr("0:1234567890123456789012345678901234567890123456789012345678901234");
    std::string raw = addr.toRaw();
    ASSERT_TRUE(!raw.empty());
    // Should start with workchain
    ASSERT_TRUE(raw.find("0:") == 0);
}

TEST(AddressWorkchain) {
    Address addr("-1:1234567890123456789012345678901234567890123456789012345678901234");
    ASSERT_TRUE(addr.isValid());
    ASSERT_EQUAL(-1, addr.getWorkchain());
}

TEST(AddressComparison) {
    Address addr1("0:1234567890123456789012345678901234567890123456789012345678901234");
    Address addr2("0:1234567890123456789012345678901234567890123456789012345678901234");
    Address addr3("0:1234567890123456789012345678901234567890123456789012345678901235");
    
    ASSERT_TRUE(addr1 == addr2);
    ASSERT_FALSE(addr1 == addr3);
    ASSERT_TRUE(addr1 != addr3);
}

int main() {
    return RUN_ALL_TESTS();
}