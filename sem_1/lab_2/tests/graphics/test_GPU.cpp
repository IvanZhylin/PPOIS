#include <gtest/gtest.h>
#include "../../include/graphics/GPU.h"
#include "../../include/graphics/VRAM.h"
#include "../../include/exceptions/Exceptions.h"

class GPUTest : public ::testing::Test {
protected:
    GPU* gpu;

    void SetUp() override {
        gpu = new GPU(2048, 1.5, 8);
    }

    void TearDown() override {
        delete gpu;
    }
};

TEST_F(GPUTest, InitialState) {
    EXPECT_EQ(gpu->getCores(), 2048);
    EXPECT_DOUBLE_EQ(gpu->getFrequency(), 1.5);
    EXPECT_LT(gpu->getTemperature(), 50.0);
}

TEST_F(GPUTest, RenderFrameIncreasesTemperature) {
    double initialTemp = gpu->getTemperature();
    gpu->renderFrame();
    EXPECT_GT(gpu->getTemperature(), initialTemp);
}

TEST_F(GPUTest, MultipleFramesIncreaseTemperature) {
    double temp1 = gpu->getTemperature();

    for (int i = 0; i < 10; i++) {
        gpu->renderFrame();
    }

    double temp2 = gpu->getTemperature();
    EXPECT_GT(temp2, temp1);
}

TEST_F(GPUTest, OverheatProtection) {
    EXPECT_THROW({
        for (int i = 0; i < 20; i++) {
            gpu->renderFrame();
        }
    }, GPUOverheatException);  
}


TEST_F(GPUTest, CoolingReducesTemperature) {
    for (int i = 0; i < 5; i++) {
        gpu->renderFrame();
    }

    double hotTemp = gpu->getTemperature();
    gpu->cool(10.0);

    EXPECT_LT(gpu->getTemperature(), hotTemp);
}

TEST_F(GPUTest, ComputeShaders) {
    double initialTemp = gpu->getTemperature();
    gpu->computeShaders();
    EXPECT_GT(gpu->getTemperature(), initialTemp);
}

TEST_F(GPUTest, VRAMAccess) {
    VRAM& vram = gpu->getVRAM();
    EXPECT_GT(vram.getCapacity(), 0);
}
