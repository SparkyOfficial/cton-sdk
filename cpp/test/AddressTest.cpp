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

TEST(AddressToUserFriendly) {
    // Create an address with known values
    std::vector<uint8_t> hashPart(32, 0x12);
    Address addr(0, hashPart);
    
    // Test user-friendly address generation
    std::string userFriendly = addr.toUserFriendly(true, false); // bounceable, not testnet
    ASSERT_TRUE(!userFriendly.empty());
    ASSERT_TRUE(userFriendly.length() >= 48); // User-friendly addresses are typically 48+ characters
    
    // Test with different parameters
    std::string userFriendly2 = addr.toUserFriendly(false, false); // non-bounceable, not testnet
    ASSERT_TRUE(!userFriendly2.empty());
    
    // Test with testnet flag
    std::string userFriendly3 = addr.toUserFriendly(true, true); // bounceable, testnet
    ASSERT_TRUE(!userFriendly3.empty());
}

TEST(AddressFromUserFriendly) {
    // Test with a known valid user-friendly address
    std::vector<uint8_t> hashPart(32, 0x12);
    Address addr(0, hashPart);
    
    // Convert to user-friendly and back
    std::string userFriendly = addr.toUserFriendly(true, false);
    if (!userFriendly.empty()) {
        Address addr2 = Address::fromUserFriendly(userFriendly);
        // Check that the addresses are equal
        ASSERT_TRUE(addr2.isValid());
        ASSERT_EQUAL(addr.getWorkchain(), addr2.getWorkchain());
        ASSERT_TRUE(addr.getHashPart() == addr2.getHashPart());
    }
}

TEST(AddressCRC16) {
    // Test that different addresses produce different CRCs
    std::vector<uint8_t> hashPart1(32, 0x12);
    std::vector<uint8_t> hashPart2(32, 0x34);
    
    Address addr1(0, hashPart1);
    Address addr2(0, hashPart2);
    
    std::string uf1 = addr1.toUserFriendly(true, false);
    std::string uf2 = addr2.toUserFriendly(true, false);
    
    // Different addresses should produce different user-friendly representations
    ASSERT_TRUE(!uf1.empty());
    ASSERT_TRUE(!uf2.empty());
    ASSERT_TRUE(uf1 != uf2);
}

TEST(AddressBase64Url) {
    // Test base64url encoding/decoding
    std::vector<uint8_t> hashPart(32, 0xFF);
    Address addr(0, hashPart);
    
    std::string userFriendly = addr.toUserFriendly();
    ASSERT_TRUE(!userFriendly.empty());
    
    // Test round-trip conversion
    Address addr2 = Address::fromUserFriendly(userFriendly);
    ASSERT_TRUE(addr2.isValid());
    ASSERT_EQUAL(addr.getWorkchain(), addr2.getWorkchain());
    ASSERT_TRUE(addr.getHashPart() == addr2.getHashPart());
}

int main() {
    return RUN_ALL_TESTS();
}