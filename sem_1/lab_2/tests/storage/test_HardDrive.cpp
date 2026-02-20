#include <gtest/gtest.h>
#include "../../include/storage/HardDrive.h"
#include "../../include/exceptions/Exceptions.h"

class HardDriveTest : public ::testing::Test {
protected:
    HardDrive* hdd;
    HardDrive* ssd;

    void SetUp() override {
        hdd = new HardDrive("Test HDD", "TestCorp", 1000, false, 49.99);
        ssd = new HardDrive("Test SSD", "TestCorp", 500, true, 99.99);
    }

    void TearDown() override {
        delete hdd;
        delete ssd;
    }
};

TEST_F(HardDriveTest, HDDProperties) {
    EXPECT_EQ(hdd->getCapacity(), 1000);
    EXPECT_FALSE(hdd->isSolidState());
    EXPECT_GT(hdd->getReadSpeed(), 100);
    EXPECT_LT(hdd->getReadSpeed(), 200);
}

TEST_F(HardDriveTest, SSDProperties) {
    EXPECT_EQ(ssd->getCapacity(), 500);
    EXPECT_TRUE(ssd->isSolidState());
    EXPECT_GT(ssd->getReadSpeed(), 1000);
}

TEST_F(HardDriveTest, WriteWhenPoweredOff) {
    EXPECT_THROW(hdd->write("test data"), std::runtime_error);
}

TEST_F(HardDriveTest, WriteWhenPoweredOn) {
    hdd->powerOn();
    EXPECT_NO_THROW(hdd->write("test data"));
}

TEST_F(HardDriveTest, ReadWhenPoweredOn) {
    hdd->powerOn();
    std::string data = hdd->read();
    EXPECT_FALSE(data.empty());
}

TEST_F(HardDriveTest, FormatPartition) {
    hdd->powerOn();
    EXPECT_NO_THROW(hdd->formatPartition(0));
}

TEST_F(HardDriveTest, InitialPartitionCount) {
    EXPECT_EQ(hdd->getPartitionCount(), 2);
}

TEST_F(HardDriveTest, CreatePartition) {
    int initialCount = hdd->getPartitionCount();
    hdd->createPartition("E:", 100);
    EXPECT_EQ(hdd->getPartitionCount(), initialCount + 1);
}

TEST_F(HardDriveTest, SSDDefragment) {
    ssd->powerOn();
    EXPECT_NO_THROW(ssd->defragment());  // SSD не требует дефрагментации
}
