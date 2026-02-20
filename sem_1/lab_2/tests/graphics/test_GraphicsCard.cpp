#include <gtest/gtest.h>
#include "../../include/graphics/GraphicsCard.h"

class GraphicsCardTest : public ::testing::Test {
protected:
    GraphicsCard* gpu;
    
    void SetUp() override {
        gpu = new GraphicsCard("RTX 4080", "NVIDIA", 9728, 2.5, 16, 1199.99);
    }
    
    void TearDown() override {
        delete gpu;
    }
};

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ КОНСТРУКТОРА И ГЕТТЕРОВ
// ═══════════════════════════════════════════════════════════════════

TEST_F(GraphicsCardTest, InitialState) {
    EXPECT_EQ(gpu->getName(), "RTX 4080");
    EXPECT_EQ(gpu->getManufacturer(), "NVIDIA");
    EXPECT_DOUBLE_EQ(gpu->getPrice(), 1199.99);
    EXPECT_EQ(gpu->getDisplayPorts(), 3);
    EXPECT_EQ(gpu->getHDMIPorts(), 1);
    EXPECT_DOUBLE_EQ(gpu->getPowerConsumption(), 250.0);
    EXPECT_FALSE(gpu->isPowered());
}

TEST_F(GraphicsCardTest, NVIDIACard) {
    GraphicsCard* nvidia = new GraphicsCard("RTX 4090", "NVIDIA", 16384, 2.6, 24, 1599.99);
    EXPECT_EQ(nvidia->getDisplayPorts(), 3);
    EXPECT_EQ(nvidia->getHDMIPorts(), 1);
    delete nvidia;
}

TEST_F(GraphicsCardTest, AMDCard) {
    GraphicsCard* amd = new GraphicsCard("RX 7900 XTX", "AMD", 12288, 2.3, 24, 999.99);
    EXPECT_EQ(amd->getManufacturer(), "AMD");
    EXPECT_EQ(amd->getDisplayPorts(), 3);
    delete amd;
}

TEST_F(GraphicsCardTest, BudgetCard) {
    GraphicsCard* budget = new GraphicsCard("RTX 3060", "NVIDIA", 3584, 1.8, 12, 329.99);
    EXPECT_DOUBLE_EQ(budget->getPowerConsumption(), 250.0);
    delete budget;
}

TEST_F(GraphicsCardTest, HighEndCard) {
    GraphicsCard* highEnd = new GraphicsCard("RTX 4090 Ti", "NVIDIA", 18432, 2.7, 48, 1999.99);
    EXPECT_DOUBLE_EQ(highEnd->getPrice(), 1999.99);
    delete highEnd;
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ RENDER 3D SCENE
// ═══════════════════════════════════════════════════════════════════

TEST_F(GraphicsCardTest, Render3DSceneWhenOff) {
    EXPECT_THROW(gpu->render3DScene(), std::runtime_error);
}

TEST_F(GraphicsCardTest, Render3DSceneWhenOn) {
    gpu->powerOn();
    EXPECT_NO_THROW(gpu->render3DScene());
}

TEST_F(GraphicsCardTest, Render3DSceneIncreasesTemperature) {
    gpu->powerOn();
    double initialTemp = gpu->getTemperature();
    
    gpu->render3DScene();
    
    EXPECT_GT(gpu->getTemperature(), initialTemp);
}

TEST_F(GraphicsCardTest, RenderMultipleScenes) {
    gpu->powerOn();
    
    gpu->render3DScene();
    gpu->render3DScene();
    gpu->render3DScene();
    
    EXPECT_TRUE(gpu->isPowered());
}

TEST_F(GraphicsCardTest, Render3DSceneAfterPowerOff) {
    gpu->powerOn();
    gpu->render3DScene();
    gpu->powerOff();
    
    EXPECT_THROW(gpu->render3DScene(), std::runtime_error);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ COMPUTE SHADERS
// ═══════════════════════════════════════════════════════════════════

TEST_F(GraphicsCardTest, ComputeShadersWhenOff) {
    EXPECT_THROW(gpu->computeShaders(), std::runtime_error);
}

TEST_F(GraphicsCardTest, ComputeShadersWhenOn) {
    gpu->powerOn();
    EXPECT_NO_THROW(gpu->computeShaders());
}

TEST_F(GraphicsCardTest, ComputeShadersIncreasesTemperature) {
    gpu->powerOn();
    double initialTemp = gpu->getTemperature();
    
    gpu->computeShaders();
    
    EXPECT_GT(gpu->getTemperature(), initialTemp);
}

TEST_F(GraphicsCardTest, ComputeMultipleTimes) {
    gpu->powerOn();
    
    gpu->computeShaders();
    gpu->computeShaders();
    gpu->computeShaders();
    
    EXPECT_TRUE(gpu->isPowered());
}

TEST_F(GraphicsCardTest, ComputeShadersAfterPowerOff) {
    gpu->powerOn();
    gpu->computeShaders();
    gpu->powerOff();
    
    EXPECT_THROW(gpu->computeShaders(), std::runtime_error);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ GET TEMPERATURE
// ═══════════════════════════════════════════════════════════════════

TEST_F(GraphicsCardTest, GetTemperatureInitial) {
    double temp = gpu->getTemperature();
    EXPECT_GE(temp, 0.0);
}

TEST_F(GraphicsCardTest, GetTemperatureAfterRendering) {
    gpu->powerOn();
    double temp1 = gpu->getTemperature();
    
    gpu->render3DScene();
    double temp2 = gpu->getTemperature();
    
    EXPECT_GT(temp2, temp1);
}

TEST_F(GraphicsCardTest, GetTemperatureAfterCompute) {
    gpu->powerOn();
    double temp1 = gpu->getTemperature();
    
    gpu->computeShaders();
    double temp2 = gpu->getTemperature();
    
    EXPECT_GT(temp2, temp1);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ COOL
// ═══════════════════════════════════════════════════════════════════

TEST_F(GraphicsCardTest, CoolGPU) {
    gpu->powerOn();
    gpu->render3DScene();
    
    double tempBefore = gpu->getTemperature();
    gpu->cool(10.0);
    double tempAfter = gpu->getTemperature();
    
    EXPECT_LT(tempAfter, tempBefore);
}

TEST_F(GraphicsCardTest, CoolMultipleTimes) {
    gpu->powerOn();
    gpu->render3DScene();
    
    gpu->cool(5.0);
    gpu->cool(5.0);
    gpu->cool(5.0);
    
    EXPECT_GE(gpu->getTemperature(), 0.0);
}

TEST_F(GraphicsCardTest, CoolWithZeroAmount) {
    gpu->powerOn();
    double temp = gpu->getTemperature();
    
    gpu->cool(0.0);
    
    EXPECT_DOUBLE_EQ(gpu->getTemperature(), temp);
}

TEST_F(GraphicsCardTest, CoolWithLargeAmount) {
    gpu->powerOn();
    gpu->cool(100.0);
    
    EXPECT_GE(gpu->getTemperature(), 0.0);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ GET DIAGNOSTICS
// ═══════════════════════════════════════════════════════════════════

TEST_F(GraphicsCardTest, GetDiagnostics) {
    std::string diag = gpu->getDiagnostics();
    
    EXPECT_NE(diag.find("RTX 4080"), std::string::npos);
    EXPECT_NE(diag.find("GPU Temp:"), std::string::npos);
    EXPECT_NE(diag.find("Cores:"), std::string::npos);
}

TEST_F(GraphicsCardTest, GetDiagnosticsAfterRendering) {
    gpu->powerOn();
    gpu->render3DScene();
    
    std::string diag = gpu->getDiagnostics();
    EXPECT_NE(diag.find("GPU Temp:"), std::string::npos);
}

TEST_F(GraphicsCardTest, GetDiagnosticsAfterCooling) {
    gpu->powerOn();
    gpu->render3DScene();
    gpu->cool(10.0);
    
    std::string diag = gpu->getDiagnostics();
    EXPECT_NE(diag.find("Cores: 9728"), std::string::npos);
}

// ═══════════════════════════════════════════════════════════════════
// ИНТЕГРАЦИОННЫЕ ТЕСТЫ
// ═══════════════════════════════════════════════════════════════════

TEST_F(GraphicsCardTest, GamingWorkload) {
    gpu->powerOn();
    
    for (int i = 0; i < 10; i++) {
        gpu->render3DScene();
    }
    
    gpu->cool(20.0);
    
    EXPECT_TRUE(gpu->isPowered());
}

TEST_F(GraphicsCardTest, ComputeWorkload) {
    gpu->powerOn();
    
    for (int i = 0; i < 10; i++) {
        gpu->computeShaders();
    }
    
    gpu->cool(15.0);
    
    EXPECT_TRUE(gpu->isPowered());
}

TEST_F(GraphicsCardTest, MixedWorkload) {
    gpu->powerOn();
    
    gpu->render3DScene();
    gpu->computeShaders();
    gpu->render3DScene();
    gpu->cool(10.0);
    gpu->computeShaders();
    
    std::string diag = gpu->getDiagnostics();
    EXPECT_FALSE(diag.empty());
}

TEST_F(GraphicsCardTest, CompleteWorkflow) {
    gpu->powerOn();
    
    gpu->render3DScene();
    double temp1 = gpu->getTemperature();
    
    gpu->computeShaders();
    double temp2 = gpu->getTemperature();
    EXPECT_GT(temp2, temp1);
    
    gpu->cool(20.0);
    EXPECT_LT(gpu->getTemperature(), temp2);
    
    std::string diag = gpu->getDiagnostics();
    EXPECT_FALSE(diag.empty());
    
    gpu->powerOff();
    EXPECT_FALSE(gpu->isPowered());
}
