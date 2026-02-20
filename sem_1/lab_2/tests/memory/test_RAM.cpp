#include <gtest/gtest.h>
#include "../../include/memory/RAM.h"
#include "../../include/exceptions/Exceptions.h"

class RAMTest : public ::testing::Test {
protected:
    RAM* ram;

    void SetUp() override {
        ram = new RAM("Test RAM", "TestCorp", 16, 3200, 99.99);
    }

    void TearDown() override {
        delete ram;
    }
};

TEST_F(RAMTest, InitialState) {
    EXPECT_EQ(ram->getTotalCapacity(), 16);
    EXPECT_EQ(ram->getUsedMemory(), 0);
    EXPECT_EQ(ram->getAvailableMemory(), 16);
    EXPECT_DOUBLE_EQ(ram->getUsagePercent(), 0.0);
}

TEST_F(RAMTest, AllocateMemory) {
    ram->allocate(4);

    EXPECT_EQ(ram->getUsedMemory(), 4);
    EXPECT_EQ(ram->getAvailableMemory(), 12);
    EXPECT_DOUBLE_EQ(ram->getUsagePercent(), 25.0);
}

TEST_F(RAMTest, MultipleAllocations) {
    ram->allocate(2);
    ram->allocate(3);
    ram->allocate(5);

    EXPECT_EQ(ram->getUsedMemory(), 10);
    EXPECT_EQ(ram->getAvailableMemory(), 6);
}

TEST_F(RAMTest, Deallocate) {
    ram->allocate(8);
    EXPECT_EQ(ram->getUsedMemory(), 8);

    ram->deallocate(3);
    EXPECT_EQ(ram->getUsedMemory(), 5);
}

TEST_F(RAMTest, DeallocateMoreThanUsed) {
    ram->allocate(5);
    ram->deallocate(10);

    EXPECT_EQ(ram->getUsedMemory(), 0);
}

TEST_F(RAMTest, MemoryOverflow) {
    EXPECT_THROW(ram->allocate(20), MemoryOverflowException);
}

TEST_F(RAMTest, ClearAll) {
    ram->allocate(10);
    ram->clearAll();

    EXPECT_EQ(ram->getUsedMemory(), 0);
}

TEST_F(RAMTest, WriteAndRead) {
    ram->powerOn();

    ram->writeData(1000, 0xAB);
    EXPECT_EQ(ram->readData(1000), 0xAB);
}

TEST_F(RAMTest, PowerState) {
    EXPECT_FALSE(ram->isPowered());

    ram->powerOn();
    EXPECT_TRUE(ram->isPowered());

    ram->powerOff();
    EXPECT_FALSE(ram->isPowered());
}
