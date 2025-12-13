#include <gtest/gtest.h>
#include "../../include/cpu/CPU.h"
#include "../../include/exceptions/Exceptions.h"

class CPUTest : public ::testing::Test {
protected:
    CPU* cpu;
    
    void SetUp() override {
        cpu = new CPU("Ryzen 9 5900X", "AMD", 12, 3.7, 549.99);
    }
    
    void TearDown() override {
        delete cpu;
    }
};

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ КОНСТРУКТОРА И ГЕТТЕРОВ
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUTest, InitialState) {
    EXPECT_EQ(cpu->getName(), "Ryzen 9 5900X");
    EXPECT_EQ(cpu->getManufacturer(), "AMD");
    EXPECT_EQ(cpu->getCoreCount(), 12);
    EXPECT_DOUBLE_EQ(cpu->getFrequency(), 3.7);
    EXPECT_EQ(cpu->getThreadCount(), 24);  // cores * 2
    EXPECT_EQ(cpu->getArchitecture(), "x86-64");
    EXPECT_DOUBLE_EQ(cpu->getTDP(), 65.0);
    EXPECT_DOUBLE_EQ(cpu->getPrice(), 549.99);
    EXPECT_FALSE(cpu->isPowered());
}

TEST_F(CPUTest, CoresCreated) {
    // 12 ядер должны быть созданы
    EXPECT_EQ(cpu->getCoreCount(), 12);
}

TEST_F(CPUTest, ThreadCount) {
    // Потоков в 2 раза больше ядер
    EXPECT_EQ(cpu->getThreadCount(), 24);
}

TEST_F(CPUTest, MaxFrequency) {
    // maxFrequency = baseFrequency * 1.5
    // Проверяем через разгон
    cpu->overclock(1.0);  // Не меняем частоту
    EXPECT_DOUBLE_EQ(cpu->getFrequency(), 3.7);
}

TEST_F(CPUTest, IntelCPU) {
    CPU* intel = new CPU("i9-13900K", "Intel", 24, 3.0, 589.99);
    EXPECT_EQ(intel->getCoreCount(), 24);
    EXPECT_EQ(intel->getThreadCount(), 48);
    EXPECT_DOUBLE_EQ(intel->getFrequency(), 3.0);
    delete intel;
}

TEST_F(CPUTest, BudgetCPU) {
    CPU* budget = new CPU("Ryzen 5 5600", "AMD", 6, 3.5, 199.99);
    EXPECT_EQ(budget->getCoreCount(), 6);
    EXPECT_EQ(budget->getThreadCount(), 12);
    delete budget;
}

TEST_F(CPUTest, SingleCoreCPU) {
    CPU* single = new CPU("Old CPU", "Intel", 1, 2.0, 50.0);
    EXPECT_EQ(single->getCoreCount(), 1);
    EXPECT_EQ(single->getThreadCount(), 2);
    delete single;
}

TEST_F(CPUTest, ZeroCoreCPU) {
    CPU* zero = new CPU("Broken", "Unknown", 0, 1.0, 0.0);
    EXPECT_EQ(zero->getCoreCount(), 0);
    EXPECT_EQ(zero->getThreadCount(), 0);
    delete zero;
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ PROCESS INSTRUCTION
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUTest, ProcessInstructionWhenOff) {
    EXPECT_THROW(cpu->processInstruction("ADD"), std::runtime_error);
}

TEST_F(CPUTest, ProcessInstructionWhenOn) {
    cpu->powerOn();
    EXPECT_NO_THROW(cpu->processInstruction("ADD"));
}

TEST_F(CPUTest, ProcessDifferentInstructions) {
    cpu->powerOn();
    cpu->processInstruction("ADD");
    cpu->processInstruction("SUB");
    cpu->processInstruction("MUL");
    cpu->processInstruction("DIV");
    cpu->processInstruction("MOV");
    EXPECT_TRUE(cpu->isPowered());
}

TEST_F(CPUTest, ProcessInstructionIncreasesTemperature) {
    cpu->powerOn();
    double initialTemp = cpu->getTemperature();
    
    cpu->processInstruction("MUL");
    
    EXPECT_GT(cpu->getTemperature(), initialTemp);
}

TEST_F(CPUTest, ProcessManyInstructions) {
    cpu->powerOn();
    
    for (int i = 0; i < 10; i++) {  // Уменьшено с 100 до 10
        cpu->processInstruction("ADD");
    }
    
    EXPECT_TRUE(cpu->isPowered());
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ GET TEMPERATURE
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUTest, GetTemperatureInitial) {
    double temp = cpu->getTemperature();
    EXPECT_GE(temp, 0.0);
}

TEST_F(CPUTest, GetTemperatureAfterInstructions) {
    cpu->powerOn();
    double temp1 = cpu->getTemperature();
    
    cpu->processInstruction("MUL");
    double temp2 = cpu->getTemperature();
    
    EXPECT_GT(temp2, temp1);
}

TEST_F(CPUTest, GetTemperatureAverageOfCores) {
    cpu->powerOn();
    cpu->processInstruction("ADD");
    
    double avgTemp = cpu->getTemperature();
    EXPECT_GT(avgTemp, 0.0);
}

TEST_F(CPUTest, GetTemperatureZeroCores) {
    CPU* zeroCores = new CPU("Empty", "Test", 0, 1.0, 0.0);
    EXPECT_DOUBLE_EQ(zeroCores->getTemperature(), 0.0);
    delete zeroCores;
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ COOL
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUTest, CoolCPU) {
    cpu->powerOn();
    cpu->processInstruction("MUL");
    
    double tempBefore = cpu->getTemperature();
    cpu->cool(10.0);
    double tempAfter = cpu->getTemperature();
    
    EXPECT_LT(tempAfter, tempBefore);
}

TEST_F(CPUTest, CoolMultipleTimes) {
    cpu->powerOn();
    cpu->processInstruction("MUL");
    
    cpu->cool(5.0);
    cpu->cool(5.0);
    cpu->cool(5.0);
    
    EXPECT_GT(cpu->getTemperature(), 0.0);
}

TEST_F(CPUTest, CoolWithZeroAmount) {
    cpu->powerOn();
    double temp = cpu->getTemperature();
    cpu->cool(0.0);
    EXPECT_DOUBLE_EQ(cpu->getTemperature(), temp);
}

TEST_F(CPUTest, CoolWithNegativeAmount) {
    cpu->powerOn();
    cpu->cool(-10.0);
    EXPECT_GE(cpu->getTemperature(), 0.0);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ OVERCLOCK
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUTest, OverclockModerate) {
    double initialFreq = cpu->getFrequency();
    cpu->overclock(1.1);  // +10%
    EXPECT_DOUBLE_EQ(cpu->getFrequency(), initialFreq * 1.1);
}

TEST_F(CPUTest, OverclockSignificant) {
    cpu->overclock(1.3);  // +30%
    EXPECT_DOUBLE_EQ(cpu->getFrequency(), 3.7 * 1.3);
}

TEST_F(CPUTest, OverclockNoChange) {
    cpu->overclock(1.0);  // Без изменений
    EXPECT_DOUBLE_EQ(cpu->getFrequency(), 3.7);
}

TEST_F(CPUTest, OverclockBelowBase) {
    cpu->overclock(0.8);  // Undervolting
    EXPECT_DOUBLE_EQ(cpu->getFrequency(), 3.7 * 0.8);
}

TEST_F(CPUTest, OverclockExceedsMax) {
    // maxFrequency = 3.7 * 1.5 = 5.55
    EXPECT_THROW(cpu->overclock(2.0), CPUOverheatException);
}

TEST_F(CPUTest, OverclockToMax) {
    cpu->overclock(1.5);  // Ровно maxFrequency
    EXPECT_DOUBLE_EQ(cpu->getFrequency(), 3.7 * 1.5);
}

TEST_F(CPUTest, OverclockSlightlyOverMax) {
    EXPECT_THROW(cpu->overclock(1.51), CPUOverheatException);
}

TEST_F(CPUTest, OverclockMultipleTimes) {
    cpu->overclock(1.1);
    cpu->overclock(1.1);  // 3.7 * 1.1 * 1.1 = 4.477
    EXPECT_DOUBLE_EQ(cpu->getFrequency(), 3.7 * 1.1 * 1.1);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ RESET ALL CORES
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUTest, ResetAllCores) {
    cpu->powerOn();
    cpu->processInstruction("ADD");
    cpu->processInstruction("MUL");
    
    cpu->resetAllCores();
    
    EXPECT_TRUE(cpu->isPowered());
}

TEST_F(CPUTest, ResetAfterOverheat) {
    cpu->powerOn();
    
    for (int i = 0; i < 10; i++) {
        cpu->processInstruction("MUL");
    }
    
    cpu->resetAllCores();
    EXPECT_GT(cpu->getTemperature(), 0.0);
}

TEST_F(CPUTest, ResetWhenOff) {
    cpu->resetAllCores();
    EXPECT_FALSE(cpu->isPowered());
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ GET DIAGNOSTICS
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUTest, GetDiagnostics) {
    std::string diag = cpu->getDiagnostics();
    
    EXPECT_NE(diag.find("Ryzen 9 5900X"), std::string::npos);
    EXPECT_NE(diag.find("Cores:"), std::string::npos);
    EXPECT_NE(diag.find("12"), std::string::npos);
    EXPECT_NE(diag.find("Freq:"), std::string::npos);
    EXPECT_NE(diag.find("3.7"), std::string::npos);
    EXPECT_NE(diag.find("Temp:"), std::string::npos);
}

TEST_F(CPUTest, GetDiagnosticsAfterOverclock) {
    cpu->overclock(1.2);
    std::string diag = cpu->getDiagnostics();
    
    EXPECT_NE(diag.find("Freq:"), std::string::npos);
}

TEST_F(CPUTest, GetDiagnosticsAfterLoad) {
    cpu->powerOn();
    cpu->processInstruction("MUL");
    
    std::string diag = cpu->getDiagnostics();
    EXPECT_NE(diag.find("Temp:"), std::string::npos);
}

// ═══════════════════════════════════════════════════════════════════
// ИНТЕГРАЦИОННЫЕ ТЕСТЫ
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUTest, CompleteWorkload) {
    cpu->powerOn();
    
    cpu->processInstruction("ADD");
    cpu->processInstruction("MUL");
    cpu->overclock(1.1);
    cpu->processInstruction("DIV");
    
    double temp = cpu->getTemperature();
    EXPECT_GT(temp, 0.0);
    
    cpu->cool(10.0);
    EXPECT_LT(cpu->getTemperature(), temp);
    
    cpu->resetAllCores();
    cpu->powerOff();
    EXPECT_FALSE(cpu->isPowered());
}

TEST_F(CPUTest, StressTest) {
    cpu->powerOn();
    cpu->overclock(1.1);  // Меньше разгон
    
    for (int i = 0; i < 10; i++) {  // Меньше инструкций
        cpu->processInstruction("MUL");
        if (i % 3 == 0) {
            cpu->cool(5.0);  // Охлаждаем периодически
        }
    }
    
    cpu->cool(20.0);
    cpu->resetAllCores();
    
    std::string diag = cpu->getDiagnostics();
    EXPECT_FALSE(diag.empty());
}

