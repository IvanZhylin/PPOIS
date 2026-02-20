#include <gtest/gtest.h>
#include "../../include/graphics/VRAM.h"
#include "../../include/exceptions/Exceptions.h"

class VRAMTest : public ::testing::Test {
protected:
    VRAM* vram;

    void SetUp() override {
        vram = new VRAM(8, 14000);
    }

    void TearDown() override {
        delete vram;
    }
};

TEST_F(VRAMTest, InitialState) {
    EXPECT_EQ(vram->getCapacity(), 8);
    EXPECT_EQ(vram->getUsed(), 0);
    EXPECT_EQ(vram->getFree(), 8 * 1024);
}

TEST_F(VRAMTest, AllocateTexture) {
    vram->allocateTexture(100);
    EXPECT_EQ(vram->getUsed(), 100);
}

TEST_F(VRAMTest, MultipleAllocations) {
    vram->allocateTexture(500);
    vram->allocateTexture(1000);
    vram->allocateTexture(200);

    EXPECT_EQ(vram->getUsed(), 1700);
}

TEST_F(VRAMTest, FreeTexture) {
    vram->allocateTexture(1000);
    vram->freeTexture(500);

    EXPECT_EQ(vram->getUsed(), 500);
}

TEST_F(VRAMTest, Clear) {
    vram->allocateTexture(2000);
    vram->clear();

    EXPECT_EQ(vram->getUsed(), 0);
}

TEST_F(VRAMTest, Overflow) {
    EXPECT_THROW(vram->allocateTexture(10000), MemoryOverflowException);
}

TEST_F(VRAMTest, UsagePercentage) {
    vram->allocateTexture(4096);  // Половина от 8 GB
    EXPECT_DOUBLE_EQ(vram->getUsagePercent(), 50.0);
}
