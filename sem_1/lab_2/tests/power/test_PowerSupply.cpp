#include <gtest/gtest.h>
#include "../../include/power/PowerSupply.h"
#include "../../include/exceptions/Exceptions.h"

class PowerSupplyTest : public ::testing::Test {
protected:
    PowerSupply* psu;

    void SetUp() override {
        psu = new PowerSupply("Test PSU", "TestCorp", 650, "80+ Gold", 89.99);
    }

    void TearDown() override {
        delete psu;
    }
};

TEST_F(PowerSupplyTest, InitialState) {
    EXPECT_EQ(psu->getWattage(), 650);
    EXPECT_DOUBLE_EQ(psu->getLoadPercent(), 0.0);
}

TEST_F(PowerSupplyTest, SupplyPower) {
    EXPECT_NO_THROW(psu->supplyPower(300));
    EXPECT_GT(psu->getLoadPercent(), 0.0);
}

TEST_F(PowerSupplyTest, LoadPercentage) {
    psu->supplyPower(325);  // 325W из 650W
    EXPECT_DOUBLE_EQ(psu->getLoadPercent(), 50.0);
}

TEST_F(PowerSupplyTest, Overload) {
    EXPECT_THROW(psu->supplyPower(700), PowerSupplyFailureException);
}

TEST_F(PowerSupplyTest, Efficiency) {
    double efficiency = psu->getEfficiency();
    EXPECT_GT(efficiency, 0.80);
    EXPECT_LE(efficiency, 1.0);
}

TEST_F(PowerSupplyTest, PowerOnOff) {
    psu->powerOn();
    EXPECT_TRUE(psu->isPowered());

    psu->powerOff();
    EXPECT_FALSE(psu->isPowered());
}
