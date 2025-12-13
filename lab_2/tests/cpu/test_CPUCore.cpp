#include <gtest/gtest.h>
#include "../../include/cpu/CPUCore.h"
#include "../../include/exceptions/Exceptions.h"

class CPUCoreTest : public ::testing::Test {
protected:
    CPUCore* core;
    
    void SetUp() override {
        core = new CPUCore(0, 3.7);
    }
    
    void TearDown() override {
        delete core;
    }
};

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ КОНСТРУКТОРА И ГЕТТЕРОВ
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUCoreTest, InitialState) {
    EXPECT_EQ(core->getId(), 0);
    EXPECT_DOUBLE_EQ(core->getFrequency(), 3.7);
    EXPECT_DOUBLE_EQ(core->getTemperature(), 35.0);
    EXPECT_FALSE(core->isActive());
}

TEST_F(CPUCoreTest, DifferentCoreIds) {
    CPUCore* core1 = new CPUCore(1, 3.5);
    CPUCore* core2 = new CPUCore(2, 4.0);
    CPUCore* core3 = new CPUCore(15, 3.8);
    
    EXPECT_EQ(core1->getId(), 1);
    EXPECT_EQ(core2->getId(), 2);
    EXPECT_EQ(core3->getId(), 15);
    
    delete core1;
    delete core2;
    delete core3;
}

TEST_F(CPUCoreTest, DifferentFrequencies) {
    CPUCore* slow = new CPUCore(0, 2.0);
    CPUCore* fast = new CPUCore(1, 5.0);
    
    EXPECT_DOUBLE_EQ(slow->getFrequency(), 2.0);
    EXPECT_DOUBLE_EQ(fast->getFrequency(), 5.0);
    
    delete slow;
    delete fast;
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ EXECUTE INSTRUCTION
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUCoreTest, ExecuteInstructionIncreasesTemperature) {
    double initialTemp = core->getTemperature();
    
    core->executeInstruction("ADD");
    
    EXPECT_DOUBLE_EQ(core->getTemperature(), initialTemp + 2.0);
}

TEST_F(CPUCoreTest, ExecuteMultipleInstructions) {
    double initialTemp = core->getTemperature();
    
    core->executeInstruction("ADD");
    core->executeInstruction("SUB");
    core->executeInstruction("MUL");
    
    EXPECT_DOUBLE_EQ(core->getTemperature(), initialTemp + 6.0);
}

TEST_F(CPUCoreTest, ExecuteDifferentInstructions) {
    core->executeInstruction("ADD");
    core->executeInstruction("SUB");
    core->executeInstruction("MUL");
    core->executeInstruction("DIV");
    core->executeInstruction("MOV");
    core->executeInstruction("JMP");
    
    EXPECT_GT(core->getTemperature(), 35.0);
}

TEST_F(CPUCoreTest, ExecuteInstructionOverheat) {
    // Temp = 35.0, каждая инструкция +2.0
    // Нужно достичь > 95.0
    // (95 - 35) / 2 = 30 инструкций
    
    for (int i = 0; i < 30; i++) {
        core->executeInstruction("MUL");
    }
    
    // 31-я должна выбросить исключение
    EXPECT_THROW(core->executeInstruction("MUL"), CPUOverheatException);
}

TEST_F(CPUCoreTest, ExecuteInstructionExactly95Degrees) {
    // Temp = 35.0
    // 95.0 - 35.0 = 60.0
    // 60.0 / 2.0 = 30 инструкций
    
    for (int i = 0; i < 30; i++) {
        core->executeInstruction("ADD");
    }
    
    EXPECT_DOUBLE_EQ(core->getTemperature(), 95.0);
    
    // Следующая выбросит исключение
    EXPECT_THROW(core->executeInstruction("ADD"), CPUOverheatException);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ COOL
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUCoreTest, CoolCore) {
    core->executeInstruction("MUL");
    
    double tempBefore = core->getTemperature();
    core->cool(5.0);
    
    EXPECT_DOUBLE_EQ(core->getTemperature(), tempBefore - 5.0);
}

TEST_F(CPUCoreTest, CoolToMinimum) {
    core->cool(20.0);  // 35 - 20 = 15, но минимум 20
    
    EXPECT_DOUBLE_EQ(core->getTemperature(), 20.0);
}

TEST_F(CPUCoreTest, CoolBelowMinimum) {
    core->cool(100.0);  // Попытка охладить ниже 20
    
    EXPECT_DOUBLE_EQ(core->getTemperature(), 20.0);
}

TEST_F(CPUCoreTest, CoolWithZeroAmount) {
    double temp = core->getTemperature();
    core->cool(0.0);
    
    EXPECT_DOUBLE_EQ(core->getTemperature(), temp);
}

TEST_F(CPUCoreTest, CoolMultipleTimes) {
    core->executeInstruction("MUL");
    core->executeInstruction("MUL");
    core->executeInstruction("MUL");
    
    core->cool(2.0);
    core->cool(2.0);
    core->cool(2.0);
    
    EXPECT_DOUBLE_EQ(core->getTemperature(), 35.0);
}

TEST_F(CPUCoreTest, CoolToExactly20) {
    // Temp = 35.0
    core->cool(15.0);
    
    EXPECT_DOUBLE_EQ(core->getTemperature(), 20.0);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ SET/IS ACTIVE
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUCoreTest, SetActiveTrue) {
    core->setActive(true);
    EXPECT_TRUE(core->isActive());
}

TEST_F(CPUCoreTest, SetActiveFalse) {
    core->setActive(false);
    EXPECT_FALSE(core->isActive());
}

TEST_F(CPUCoreTest, SetActiveToggle) {
    core->setActive(true);
    EXPECT_TRUE(core->isActive());
    
    core->setActive(false);
    EXPECT_FALSE(core->isActive());
    
    core->setActive(true);
    EXPECT_TRUE(core->isActive());
}

TEST_F(CPUCoreTest, SetActiveMultipleTimes) {
    core->setActive(true);
    core->setActive(true);
    core->setActive(true);
    
    EXPECT_TRUE(core->isActive());
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ RESET
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUCoreTest, ResetTemperature) {
    core->executeInstruction("MUL");
    core->executeInstruction("MUL");
    
    core->reset();
    
    EXPECT_DOUBLE_EQ(core->getTemperature(), 35.0);
}

TEST_F(CPUCoreTest, ResetActiveFlag) {
    core->setActive(true);
    
    core->reset();
    
    EXPECT_FALSE(core->isActive());
}

TEST_F(CPUCoreTest, ResetClearsRegisters) {
    core->executeInstruction("MOV");
    
    core->reset();
    
    EXPECT_DOUBLE_EQ(core->getTemperature(), 35.0);
    EXPECT_FALSE(core->isActive());
}

TEST_F(CPUCoreTest, ResetAfterOverheating) {
    for (int i = 0; i < 30; i++) {
        core->executeInstruction("MUL");
    }
    
    core->reset();
    
    EXPECT_DOUBLE_EQ(core->getTemperature(), 35.0);
}

TEST_F(CPUCoreTest, ResetMultipleTimes) {
    core->executeInstruction("ADD");
    core->reset();
    core->executeInstruction("SUB");
    core->reset();
    
    EXPECT_DOUBLE_EQ(core->getTemperature(), 35.0);
}

// ═══════════════════════════════════════════════════════════════════
// ИНТЕГРАЦИОННЫЕ ТЕСТЫ
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUCoreTest, CompleteWorkload) {
    core->setActive(true);
    
    core->executeInstruction("ADD");
    core->executeInstruction("MUL");
    
    EXPECT_GT(core->getTemperature(), 35.0);
    
    core->cool(2.0);
    
    core->reset();
    
    EXPECT_DOUBLE_EQ(core->getTemperature(), 35.0);
    EXPECT_FALSE(core->isActive());
}


TEST_F(CPUCoreTest, StressTest) {
    core->setActive(true);
    
    for (int i = 0; i < 10; i++) {
        core->executeInstruction("MUL");  // +20°C итого (55°C)
    }
    
    EXPECT_GT(core->getTemperature(), 50.0);
    EXPECT_LT(core->getTemperature(), 95.0);  // Не перегрелось
    
    core->cool(30.0);
    
    EXPECT_GE(core->getTemperature(), 20.0);  // Минимум 20°C
}

TEST_F(CPUCoreTest, HeatAndCoolCycle) {
    for (int cycle = 0; cycle < 5; cycle++) {
        for (int i = 0; i < 10; i++) {
            core->executeInstruction("ADD");
        }
        
        core->cool(10.0);
    }
    
    EXPECT_GT(core->getTemperature(), 20.0);
}

TEST_F(CPUCoreTest, MaxTemperatureAndReset) {
    for (int i = 0; i < 30; i++) {
        core->executeInstruction("MUL");
    }
    
    EXPECT_DOUBLE_EQ(core->getTemperature(), 95.0);
    
    core->reset();
    
    EXPECT_DOUBLE_EQ(core->getTemperature(), 35.0);
    
    // Можем снова выполнять инструкции
    EXPECT_NO_THROW(core->executeInstruction("ADD"));
}

TEST_F(CPUCoreTest, ActivateExecuteDeactivate) {
    core->setActive(true);
    EXPECT_TRUE(core->isActive());
    
    core->executeInstruction("ADD");
    core->executeInstruction("MUL");
    
    core->setActive(false);
    EXPECT_FALSE(core->isActive());
    
    core->reset();
    EXPECT_FALSE(core->isActive());
}
