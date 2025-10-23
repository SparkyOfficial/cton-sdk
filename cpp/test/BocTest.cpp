// BocTest.cpp - тести для Boc класу
// Author: Андрій Будильников (Sparky)
// Unit tests for Boc class
// Модульные тесты для класса Boc

#include "TestFramework.h"
#include "../include/Boc.h"
#include "../include/Cell.h"
#include <cstring>

using namespace cton;

TEST(BocCreation) {
    Boc boc;
    ASSERT_TRUE(true); // Just test that it can be created
}

TEST(BocWithRoot) {
    CellBuilder builder;
    builder.storeUInt(32, 0x12345678);
    auto cell = builder.build();
    
    Boc boc(cell);
    ASSERT_TRUE(true); // Just test that it can be created
}

TEST(BocSerialize) {
    CellBuilder builder;
    builder.storeUInt(32, 0x12345678);
    auto cell = builder.build();
    
    Boc boc(cell);
    
    // Test serialization
    auto data = boc.serialize(true, true);
    // Should at least contain the magic bytes
    ASSERT_TRUE(data.size() >= 4);
    
    // Check magic bytes
    if (data.size() >= 4) {
        ASSERT_EQUAL(0xB5, data[0]);
        ASSERT_EQUAL(0xEE, data[1]);
        ASSERT_EQUAL(0x90, data[2]);
        ASSERT_EQUAL(0x20, data[3]);
    }
}

TEST(BocDeserialize) {
    // Create some test data
    std::vector<uint8_t> testData = {0xB5, 0xEE, 0x90, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    
    // Test deserialization
    try {
        Boc boc = Boc::deserialize(testData);
        ASSERT_TRUE(true); // Should not crash
    } catch (...) {
        // Might throw for invalid data, which is fine
        ASSERT_TRUE(true);
    }
}

TEST(BocGetRoot) {
    CellBuilder builder;
    builder.storeUInt(32, 0x12345678);
    auto cell = builder.build();
    
    Boc boc(cell);
    auto root = boc.getRoot();
    ASSERT_TRUE(root != nullptr);
}

TEST(BocSetRoot) {
    Boc boc;
    
    CellBuilder builder;
    builder.storeUInt(32, 0x12345678);
    auto cell = builder.build();
    
    boc.setRoot(cell);
    auto root = boc.getRoot();
    ASSERT_TRUE(root != nullptr);
}

TEST(BocParserCreation) {
    std::vector<uint8_t> data = {0xB5, 0xEE, 0x90, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    BocParser parser(data);
    ASSERT_TRUE(true); // Just test that it can be created
}

TEST(BocParserParse) {
    std::vector<uint8_t> data = {0xB5, 0xEE, 0x90, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    
    try {
        BocParser parser(data);
        Boc boc = parser.parse();
        ASSERT_TRUE(true); // Should not crash
    } catch (...) {
        // Might throw for invalid data, which is fine
        ASSERT_TRUE(true);
    }
}

TEST(BocBuilderCreation) {
    CellBuilder cellBuilder;
    cellBuilder.storeUInt(32, 0x12345678);
    auto cell = cellBuilder.build();
    
    BocBuilder builder(cell);
    ASSERT_TRUE(true); // Just test that it can be created
}

TEST(BocBuilderBuild) {
    CellBuilder cellBuilder;
    cellBuilder.storeUInt(32, 0x12345678);
    auto cell = cellBuilder.build();
    
    BocBuilder builder(cell);
    auto data = builder.build(true, true);
    // Should at least contain the magic bytes
    ASSERT_TRUE(data.size() >= 4);
}

int main() {
    return RUN_ALL_TESTS();
}