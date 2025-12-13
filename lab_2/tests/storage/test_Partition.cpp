#include <gtest/gtest.h>
#include "../../include/storage/Partition.h"
#include "../../include/exceptions/Exceptions.h"

class PartitionTest : public ::testing::Test {
protected:
    Partition* partition;

    void SetUp() override {
        partition = new Partition("C:", 500, "NTFS");
    }

    void TearDown() override {
        delete partition;
    }
};

TEST_F(PartitionTest, InitialState) {
    EXPECT_EQ(partition->getLabel(), "C:");
    EXPECT_EQ(partition->getSize(), 500);
    EXPECT_EQ(partition->getUsed(), 0);
    EXPECT_EQ(partition->getFree(), 500);
    EXPECT_EQ(partition->getFilesystem(), "NTFS");
}

TEST_F(PartitionTest, WriteFile) {
    partition->writeFile("test.txt", 10);
    EXPECT_EQ(partition->getUsed(), 10);
    EXPECT_EQ(partition->getFree(), 490);
}

TEST_F(PartitionTest, MultipleFiles) {
    partition->writeFile("file1.txt", 50);
    partition->writeFile("file2.txt", 100);
    partition->writeFile("file3.txt", 25);

    EXPECT_EQ(partition->getUsed(), 175);
}

TEST_F(PartitionTest, DiskFull) {
    EXPECT_THROW(partition->writeFile("large.bin", 600), DiskFullException);
}

TEST_F(PartitionTest, DeleteFile) {
    partition->writeFile("file.txt", 100);
    partition->deleteFile(100);

    EXPECT_EQ(partition->getUsed(), 0);
}

TEST_F(PartitionTest, Format) {
    partition->writeFile("file1.txt", 100);
    partition->writeFile("file2.txt", 200);

    partition->format();

    EXPECT_EQ(partition->getUsed(), 0);
    EXPECT_EQ(partition->getFree(), 500);
}

TEST_F(PartitionTest, UsagePercentage) {
    partition->writeFile("file.txt", 250);
    EXPECT_DOUBLE_EQ(partition->getUsagePercent(), 50.0);
}
