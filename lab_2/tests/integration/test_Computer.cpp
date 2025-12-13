#include <gtest/gtest.h>
#include "../../include/Computer.h"
#include "../../include/exceptions/Exceptions.h"

class ComputerTest : public ::testing::Test {
protected:
    Computer* computer;
    
    void SetUp() override {
        computer = new Computer("TestPC");
        computer->assembleParts();
    }
    
    void TearDown() override {
        delete computer;
    }
};

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ КОНСТРУКТОРА И БАЗОВЫХ МЕТОДОВ
// ═══════════════════════════════════════════════════════════════════

TEST_F(ComputerTest, InitialState) {
    Computer* pc = new Computer("MyPC");
    EXPECT_EQ(pc->getName(), "MyPC");
    EXPECT_FALSE(pc->isOn());
    delete pc;
}

TEST_F(ComputerTest, NameAssignment) {
    EXPECT_EQ(computer->getName(), "TestPC");
}

TEST_F(ComputerTest, AssemblePartsCreatesComponents) {
    Computer* pc = new Computer("NewPC");
    pc->assembleParts();
    EXPECT_FALSE(pc->isOn());
    delete pc;
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ POWER ON/OFF
// ═══════════════════════════════════════════════════════════════════

TEST_F(ComputerTest, PowerOn) {
    computer->powerOn();
    EXPECT_TRUE(computer->isOn());
}

TEST_F(ComputerTest, PowerOnWhenAlreadyOn) {
    computer->powerOn();
    computer->powerOn();  // Второй раз - ничего не происходит
    EXPECT_TRUE(computer->isOn());
}

TEST_F(ComputerTest, PowerOff) {
    computer->powerOn();
    computer->powerOff();
    EXPECT_FALSE(computer->isOn());
}

TEST_F(ComputerTest, PowerOffWhenAlreadyOff) {
    computer->powerOff();
    computer->powerOff();  // Уже выключен
    EXPECT_FALSE(computer->isOn());
}

TEST_F(ComputerTest, PowerCycle) {
    computer->powerOn();
    EXPECT_TRUE(computer->isOn());
    
    computer->powerOff();
    EXPECT_FALSE(computer->isOn());
    
    computer->powerOn();
    EXPECT_TRUE(computer->isOn());
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ RESTART
// ═══════════════════════════════════════════════════════════════════

TEST_F(ComputerTest, Restart) {
    computer->powerOn();
    computer->restart();
    EXPECT_TRUE(computer->isOn());
}

TEST_F(ComputerTest, RestartWhenOff) {
    computer->restart();
    EXPECT_TRUE(computer->isOn());
}

TEST_F(ComputerTest, MultipleRestarts) {
    computer->powerOn();
    computer->restart();
    computer->restart();
    computer->restart();
    EXPECT_TRUE(computer->isOn());
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ PERFORM CPU TASK
// ═══════════════════════════════════════════════════════════════════

TEST_F(ComputerTest, PerformCPUTaskWhenOff) {
    EXPECT_THROW(computer->performCPUTask(), std::runtime_error);
}

TEST_F(ComputerTest, PerformCPUTaskWhenOn) {
    computer->powerOn();
    EXPECT_NO_THROW(computer->performCPUTask());
}

TEST_F(ComputerTest, PerformMultipleCPUTasks) {
    computer->powerOn();
    computer->performCPUTask();
    computer->performCPUTask();
    computer->performCPUTask();
    EXPECT_TRUE(computer->isOn());
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ RENDER GRAPHICS
// ═══════════════════════════════════════════════════════════════════

TEST_F(ComputerTest, RenderGraphicsWhenOff) {
    EXPECT_THROW(computer->renderGraphics(), std::runtime_error);
}

TEST_F(ComputerTest, RenderGraphicsWhenOn) {
    computer->powerOn();
    EXPECT_NO_THROW(computer->renderGraphics());
}

TEST_F(ComputerTest, RenderMultipleFrames) {
    computer->powerOn();
    computer->renderGraphics();
    computer->renderGraphics();
    computer->renderGraphics();
    EXPECT_TRUE(computer->isOn());
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ ALLOCATE MEMORY
// ═══════════════════════════════════════════════════════════════════

TEST_F(ComputerTest, AllocateMemoryWhenOff) {
    EXPECT_THROW(computer->allocateMemory(8), std::runtime_error);
}

TEST_F(ComputerTest, AllocateMemoryWhenOn) {
    computer->powerOn();
    EXPECT_NO_THROW(computer->allocateMemory(8));
}

TEST_F(ComputerTest, AllocateMemoryMultipleTimes) {
    computer->powerOn();
    computer->allocateMemory(4);
    computer->allocateMemory(8);
    computer->allocateMemory(2);
    EXPECT_TRUE(computer->isOn());
}

TEST_F(ComputerTest, AllocateMemoryOverflow) {
    computer->powerOn();
    // RAM 32GB, пытаемся выделить 64GB
    computer->allocateMemory(64);  // Catch MemoryOverflowException
    EXPECT_TRUE(computer->isOn());
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ SAVE TO STORAGE
// ═══════════════════════════════════════════════════════════════════

TEST_F(ComputerTest, SaveToStorageWhenOff) {
    EXPECT_THROW(computer->saveToStorage("data.txt"), std::runtime_error);
}

TEST_F(ComputerTest, SaveToStorageWhenOn) {
    computer->powerOn();
    EXPECT_NO_THROW(computer->saveToStorage("file.txt"));
}

TEST_F(ComputerTest, SaveMultipleFiles) {
    computer->powerOn();
    computer->saveToStorage("file1.txt");
    computer->saveToStorage("file2.doc");
    computer->saveToStorage("file3.pdf");
    EXPECT_TRUE(computer->isOn());
}

TEST_F(ComputerTest, SaveToStorageDiskFull) {
    computer->powerOn();
    
    // Заполняем диск (1000GB)
    for (int i = 0; i < 1000; i++) {
        computer->saveToStorage("big_file_" + std::to_string(i));
    }
    
    EXPECT_TRUE(computer->isOn());
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ CONNECT TO NETWORK
// ═══════════════════════════════════════════════════════════════════

TEST_F(ComputerTest, ConnectToNetworkWhenOff) {
    EXPECT_THROW(computer->connectToNetwork(), std::runtime_error);
}

TEST_F(ComputerTest, ConnectToNetworkWhenOn) {
    computer->powerOn();
    EXPECT_NO_THROW(computer->connectToNetwork());
}

TEST_F(ComputerTest, ConnectToNetworkMultipleTimes) {
    computer->powerOn();
    computer->connectToNetwork();
    computer->connectToNetwork();
    computer->connectToNetwork();
    EXPECT_TRUE(computer->isOn());
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ GET SYSTEM INFO
// ═══════════════════════════════════════════════════════════════════

TEST_F(ComputerTest, GetSystemInfoWhenOff) {
    std::string info = computer->getSystemInfo();
    EXPECT_NE(info.find("TestPC"), std::string::npos);
    EXPECT_NE(info.find("Status: OFF"), std::string::npos);
}

TEST_F(ComputerTest, GetSystemInfoWhenOn) {
    computer->powerOn();
    std::string info = computer->getSystemInfo();
    
    EXPECT_NE(info.find("TestPC"), std::string::npos);
    EXPECT_NE(info.find("Status: ON"), std::string::npos);
    EXPECT_NE(info.find("Power:"), std::string::npos);
}

TEST_F(ComputerTest, GetSystemInfoContainsCPU) {
    std::string info = computer->getSystemInfo();
    EXPECT_NE(info.find("Ryzen 9 5900X"), std::string::npos);
}

TEST_F(ComputerTest, GetSystemInfoContainsRAM) {
    std::string info = computer->getSystemInfo();
    EXPECT_NE(info.find("Vengeance RGB"), std::string::npos);
}

TEST_F(ComputerTest, GetSystemInfoContainsGPU) {
    std::string info = computer->getSystemInfo();
    EXPECT_NE(info.find("RTX 4080"), std::string::npos);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ RUN DIAGNOSTICS
// ═══════════════════════════════════════════════════════════════════

TEST_F(ComputerTest, RunDiagnostics) {
    computer->powerOn();
    EXPECT_NO_THROW(computer->runDiagnostics());
}

TEST_F(ComputerTest, RunDiagnosticsWhenOff) {
    EXPECT_NO_THROW(computer->runDiagnostics());
}

TEST_F(ComputerTest, RunDiagnosticsMultipleTimes) {
    computer->powerOn();
    computer->runDiagnostics();
    computer->runDiagnostics();
    computer->runDiagnostics();
    EXPECT_TRUE(computer->isOn());
}

// ═══════════════════════════════════════════════════════════════════
// ИНТЕГРАЦИОННЫЕ ТЕСТЫ
// ═══════════════════════════════════════════════════════════════════

TEST_F(ComputerTest, CompleteWorkflow) {
    // Включение
    computer->powerOn();
    EXPECT_TRUE(computer->isOn());
    
    // CPU задачи
    computer->performCPUTask();
    
    // Графика
    computer->renderGraphics();
    
    // Память
    computer->allocateMemory(8);
    
    // Хранилище
    computer->saveToStorage("document.txt");
    
    // Сеть
    computer->connectToNetwork();
    
    // Диагностика
    computer->runDiagnostics();
    
    // Выключение
    computer->powerOff();
    EXPECT_FALSE(computer->isOn());
}

TEST_F(ComputerTest, GamingSession) {
    computer->powerOn();
    
    // Игровая сессия
    for (int i = 0; i < 10; i++) {
        computer->renderGraphics();
        computer->performCPUTask();
    }
    
    computer->allocateMemory(16);
    computer->saveToStorage("savegame.dat");
    
    EXPECT_TRUE(computer->isOn());
}

TEST_F(ComputerTest, WorkSession) {
    computer->powerOn();
    
    computer->allocateMemory(8);
    computer->saveToStorage("report.docx");
    computer->saveToStorage("spreadsheet.xlsx");
    computer->saveToStorage("presentation.pptx");
    computer->connectToNetwork();
    
    EXPECT_TRUE(computer->isOn());
}

TEST_F(ComputerTest, StressTest) {
    computer->powerOn();
    
    for (int i = 0; i < 100; i++) {
        computer->performCPUTask();
    }
    
    for (int i = 0; i < 50; i++) {
        computer->renderGraphics();
    }
    
    computer->runDiagnostics();
    EXPECT_TRUE(computer->isOn());
}

TEST_F(ComputerTest, PowerFailureRecovery) {
    computer->powerOn();
    computer->performCPUTask();
    computer->allocateMemory(8);
    
    computer->powerOff();
    EXPECT_FALSE(computer->isOn());
    
    computer->powerOn();
    EXPECT_TRUE(computer->isOn());
}

TEST_F(ComputerTest, MultipleOperationsBeforeRestart) {
    computer->powerOn();
    computer->performCPUTask();
    computer->renderGraphics();
    computer->allocateMemory(4);
    
    computer->restart();
    
    EXPECT_TRUE(computer->isOn());
}

TEST_F(ComputerTest, AllOperationsWhenOff) {
    // Все должны выбросить исключение
    EXPECT_THROW(computer->performCPUTask(), std::runtime_error);
    EXPECT_THROW(computer->renderGraphics(), std::runtime_error);
    EXPECT_THROW(computer->allocateMemory(8), std::runtime_error);
    EXPECT_THROW(computer->saveToStorage("file.txt"), std::runtime_error);
    EXPECT_THROW(computer->connectToNetwork(), std::runtime_error);
    
    EXPECT_FALSE(computer->isOn());
}

TEST_F(ComputerTest, DestructorWhenOn) {
    Computer* pc = new Computer("TempPC");
    pc->assembleParts();
    pc->powerOn();
    EXPECT_TRUE(pc->isOn());
    delete pc;  // Деструктор должен выключить
    // Если не упало - OK
}

TEST_F(ComputerTest, DestructorWhenOff) {
    Computer* pc = new Computer("TempPC");
    pc->assembleParts();
    EXPECT_FALSE(pc->isOn());
    delete pc;
}
