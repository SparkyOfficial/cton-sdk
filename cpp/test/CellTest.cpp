// CellTest.cpp - тести для Cell класу
// Author: Андрій Будильников (Sparky)
// Unit tests for Cell class
// Модульные тесты для класса Cell

#include "TestFramework.h"
#include "../include/Cell.h"
#include <cstring>

using namespace cton;

TEST(CellCreation) {
    Cell cell;
    ASSERT_EQUAL(0, cell.getBitSize());
    
    auto data = cell.getData();
    ASSERT_EQUAL(0, data.size());
    
    auto refs = cell.getReferences();
    ASSERT_EQUAL(0, refs.size());
}

TEST(CellBuilderCreation) {
    CellBuilder builder;
    // Just test that it can be created
    ASSERT_TRUE(true);
}

TEST(StoreUInt) {
    CellBuilder builder;
    builder.storeUInt(32, 0x12345678);
    
    auto cell = builder.build();
    ASSERT_EQUAL(32, cell->getBitSize());
}

TEST(StoreInt) {
    CellBuilder builder;
    builder.storeInt(32, -100);
    
    auto cell = builder.build();
    ASSERT_EQUAL(32, cell->getBitSize());
}

TEST(StoreBytes) {
    CellBuilder builder;
    std::vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04};
    builder.storeBytes(data);
    
    auto cell = builder.build();
    ASSERT_EQUAL(32, cell->getBitSize()); // 4 bytes = 32 bits
}

TEST(StoreRef) {
    CellBuilder builder1;
    builder1.storeUInt(8, 0xFF);
    auto cell1 = builder1.build();
    
    CellBuilder builder2;
    builder2.storeUInt(8, 0x00);
    builder2.storeRef(cell1);
    
    auto cell2 = builder2.build();
    ASSERT_EQUAL(1, cell2->getReferences().size());
}

TEST(MaxBits) {
    CellBuilder builder;
    // Test that we can store up to MAX_BITS
    builder.storeUInt(Cell::MAX_BITS, 0);
    
    auto cell = builder.build();
    ASSERT_EQUAL(Cell::MAX_BITS, cell->getBitSize());
}

TEST(OverflowProtection) {
    CellBuilder builder;
    // Try to store more than MAX_BITS - should throw
    try {
        builder.storeUInt(Cell::MAX_BITS, 0);
        builder.storeUInt(1, 1); // This should overflow
        ASSERT_TRUE(false); // Should not reach here
    } catch (...) {
        ASSERT_TRUE(true); // Expected exception
    }
}

int main() {
    return RUN_ALL_TESTS();
}