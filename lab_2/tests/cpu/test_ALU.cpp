#include <gtest/gtest.h>
#include "../../include/cpu/ALU.h"

class ALUTest : public ::testing::Test {
protected:
    ALU* alu;

    void SetUp() override {
        alu = new ALU(64);
    }

    void TearDown() override {
        delete alu;
    }
};

TEST_F(ALUTest, AdditionBasic) {
    uint64_t result = alu->add(5, 3);
    EXPECT_EQ(result, 8);
}

TEST_F(ALUTest, AdditionWithZero) {
    uint64_t result = alu->add(10, 0);
    EXPECT_EQ(result, 10);
}

TEST_F(ALUTest, AdditionZeroFlag) {
    alu->add(0, 0);
    EXPECT_TRUE(alu->getZeroFlag());
}

TEST_F(ALUTest, SubtractionBasic) {
    uint64_t result = alu->subtract(10, 3);
    EXPECT_EQ(result, 7);
}

TEST_F(ALUTest, MultiplicationBasic) {
    uint64_t result = alu->multiply(4, 5);
    EXPECT_EQ(result, 20);
}

TEST_F(ALUTest, DivisionBasic) {
    uint64_t result = alu->divide(20, 4);
    EXPECT_EQ(result, 5);
}

TEST_F(ALUTest, DivisionByZero) {
    EXPECT_THROW(alu->divide(10, 0), std::runtime_error);
}

TEST_F(ALUTest, BitwiseAndOperation) {
    uint64_t result = alu->bitwiseAnd(0b1100, 0b1010);
    EXPECT_EQ(result, 0b1000);
}

TEST_F(ALUTest, BitwiseOrOperation) {
    uint64_t result = alu->bitwiseOr(0b1100, 0b1010);
    EXPECT_EQ(result, 0b1110);
}

TEST_F(ALUTest, LargeNumbers) {
    uint64_t a = 1000000000ULL;
    uint64_t b = 2000000000ULL;
    uint64_t result = alu->add(a, b);
    EXPECT_EQ(result, 3000000000ULL);
}
