#include <gtest/gtest.h>
#include "../../include/memory/MemoryModule.h"
#include "../../include/exceptions/Exceptions.h"

class MemoryModuleTest : public ::testing::Test {
protected:
    MemoryModule* module;

    void SetUp() override {
        module = new MemoryModule(8, 3200, "DDR4");
    }

    void TearDown() override {
        delete module;
    }
};

TEST_F(MemoryModuleTest, InitialState) {
    EXPECT_EQ(module->getCapacity(), 8);
    EXPECT_EQ(module->getFrequency(), 3200);
    EXPECT_EQ(module->getType(), "DDR4");
    EXPECT_EQ(module->getUsedBytes(), 0);
}

TEST_F(MemoryModuleTest, WriteAndRead) {
    module->write(1000, 0xAB);
    EXPECT_EQ(module->read(1000), 0xAB);
}

TEST_F(MemoryModuleTest, MultipleWrites) {
    module->write(100, 0x11);
    module->write(200, 0x22);
    module->write(300, 0x33);

    EXPECT_EQ(module->read(100), 0x11);
    EXPECT_EQ(module->read(200), 0x22);
    EXPECT_EQ(module->read(300), 0x33);
}

TEST_F(MemoryModuleTest, ReadUnwrittenAddress) {
    EXPECT_EQ(module->read(999), 0);
}

TEST_F(MemoryModuleTest, OverwriteValue) {
    module->write(500, 0xAA);
    EXPECT_EQ(module->read(500), 0xAA);

    module->write(500, 0xBB);
    EXPECT_EQ(module->read(500), 0xBB);
}

TEST_F(MemoryModuleTest, ClearMemory) {
    module->write(100, 0xFF);
    module->write(200, 0xEE);

    module->clear();

    EXPECT_EQ(module->getUsedBytes(), 0);
    EXPECT_EQ(module->read(100), 0);
}

TEST_F(MemoryModuleTest, OutOfBoundsWrite) {
    uint64_t maxAddress = 8ULL * 1024ULL * 1024ULL * 1024ULL;
    EXPECT_THROW(module->write(maxAddress, 0xFF), MemoryOverflowException);
}

TEST_F(MemoryModuleTest, OutOfBoundsRead) {
    uint64_t maxAddress = 8ULL * 1024ULL * 1024ULL * 1024ULL;
    EXPECT_THROW(module->read(maxAddress), MemoryOverflowException);
}

TEST_F(MemoryModuleTest, UsedBytesTracking) {
    EXPECT_EQ(module->getUsedBytes(), 0);

    module->write(100, 0x11);
    EXPECT_EQ(module->getUsedBytes(), 1);

    module->write(200, 0x22);
    EXPECT_EQ(module->getUsedBytes(), 2);

    // Перезапись не увеличивает счётчик
    module->write(100, 0x33);
    EXPECT_EQ(module->getUsedBytes(), 2);
}
