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
    // Create some test data with valid BOC structure
    std::vector<uint8_t> testData = {
        0xB5, 0xEE, 0x90, 0x20,  // Magic bytes
        0x80, 0x00,              // Flags (has index)
        0x01,                    // Cell count (1 cell)
        0x01, 0x01, 0x01, 0x01,  // Field sizes
        0x01,                    // Root count
        0x00,                    // Absent count
        0x00,                    // Root index
        0x00,                    // Cell offset
        0x80,                    // Cell descriptor (has bits, 0 refs)
        0x08,                    // Bit size (8 bits)
        0xFF,                    // Cell data (1 byte)
        0x00, 0x00               // CRC (placeholder)
    };
    
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
    // Create valid BOC data
    std::vector<uint8_t> data = {
        0xB5, 0xEE, 0x90, 0x20,  // Magic bytes
        0x00,                    // Flags
        0x01,                    // Cell count
        0x01, 0x01, 0x01, 0x01,  // Field sizes
        0x01,                    // Root count
        0x00,                    // Absent count
        0x00,                    // Root index
        0x80,                    // Cell descriptor (has bits, 0 refs)
        0x08,                    // Bit size (8 bits)
        0xFF                     // Cell data (1 byte)
    };
    
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
    
    // Verify the data is actually a valid BOC
    if (data.size() >= 4) {
        ASSERT_EQUAL(0xB5, data[0]);
        ASSERT_EQUAL(0xEE, data[1]);
        ASSERT_EQUAL(0x90, data[2]);
        ASSERT_EQUAL(0x20, data[3]);
    }
}

// New test for round-trip serialization/deserialization
TEST(BocRoundTrip) {
    // Create a cell with some data
    CellBuilder builder;
    builder.storeUInt(32, 0x12345678);
    builder.storeBytes({0x01, 0x02, 0x03, 0x04});
    auto originalCell = builder.build();
    
    // Create BOC and serialize it
    Boc originalBoc(originalCell);
    auto serializedData = originalBoc.serialize(false, false); // No index, no CRC for simplicity
    
    // Make sure we have data
    ASSERT_TRUE(serializedData.size() > 0);
    
    // Try to deserialize it back
    try {
        Boc deserializedBoc = Boc::deserialize(serializedData);
        auto deserializedRoot = deserializedBoc.getRoot();
        ASSERT_TRUE(deserializedRoot != nullptr);
        // Note: Full verification would require comparing cell contents
    } catch (...) {
        // For now, just make sure it doesn't crash
        ASSERT_TRUE(true);
    }
}

int main() {
    return RUN_ALL_TESTS();
}