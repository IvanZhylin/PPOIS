#include <gtest/gtest.h>
#include "../../include/os/OperatingSystem.h"

class OperatingSystemTest : public ::testing::Test {
protected:
    OperatingSystem* os;
    
    void SetUp() override {
        os = new OperatingSystem("Windows", "11");
    }
    
    void TearDown() override {
        delete os;
    }
};

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ КОНСТРУКТОРА И ГЕТТЕРОВ
// ═══════════════════════════════════════════════════════════════════

TEST_F(OperatingSystemTest, InitialState) {
    EXPECT_EQ(os->getName(), "Windows");
    EXPECT_FALSE(os->isRunning());
}

TEST_F(OperatingSystemTest, Windows11) {
    OperatingSystem* win = new OperatingSystem("Windows", "11");
    EXPECT_EQ(win->getName(), "Windows");
    delete win;
}



TEST_F(OperatingSystemTest, MacOS) {
    OperatingSystem* mac = new OperatingSystem("macOS", "Sonoma");
    EXPECT_EQ(mac->getName(), "macOS");
    delete mac;
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ BOOT
// ═══════════════════════════════════════════════════════════════════

TEST_F(OperatingSystemTest, Boot) {
    os->boot();
    EXPECT_TRUE(os->isRunning());
}

TEST_F(OperatingSystemTest, BootCreatesSystemProcess) {
    os->boot();
    EXPECT_TRUE(os->isRunning());
}

TEST_F(OperatingSystemTest, BootMultipleTimes) {
    os->boot();
    os->boot();
    os->boot();
    EXPECT_TRUE(os->isRunning());
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ SHUTDOWN
// ═══════════════════════════════════════════════════════════════════

TEST_F(OperatingSystemTest, ShutdownWhenRunning) {
    os->boot();
    os->shutdown();
    EXPECT_FALSE(os->isRunning());
}

TEST_F(OperatingSystemTest, ShutdownWhenNotRunning) {
    os->shutdown();
    EXPECT_FALSE(os->isRunning());
}

TEST_F(OperatingSystemTest, ShutdownTerminatesProcesses) {
    os->boot();
    os->startProcess("Chrome", 500);
    os->startProcess("VSCode", 300);
    
    os->shutdown();
    
    EXPECT_FALSE(os->isRunning());
}

TEST_F(OperatingSystemTest, ShutdownEmptyProcessList) {
    os->shutdown();
    EXPECT_FALSE(os->isRunning());
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ START PROCESS
// ═══════════════════════════════════════════════════════════════════

TEST_F(OperatingSystemTest, StartSingleProcess) {
    os->boot();
    os->startProcess("Chrome", 512);
    EXPECT_TRUE(os->isRunning());
}

TEST_F(OperatingSystemTest, StartMultipleProcesses) {
    os->boot();
    os->startProcess("Chrome", 500);
    os->startProcess("VSCode", 300);
    os->startProcess("Spotify", 200);
    EXPECT_TRUE(os->isRunning());
}

TEST_F(OperatingSystemTest, StartProcessWithZeroMemory) {
    os->boot();
    os->startProcess("Notepad", 0);
    EXPECT_TRUE(os->isRunning());
}

TEST_F(OperatingSystemTest, StartProcessWithLargeMemory) {
    os->boot();
    os->startProcess("Game", 8192);
    EXPECT_TRUE(os->isRunning());
}

TEST_F(OperatingSystemTest, StartProcessBeforeBoot) {
    os->startProcess("Process", 100);
    // Не должно упасть
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ KILL PROCESS
// ═══════════════════════════════════════════════════════════════════

TEST_F(OperatingSystemTest, KillExistingProcess) {
    os->boot();  // PID 0: System
    os->startProcess("Chrome", 500);  // PID 1
    
    os->killProcess(1);
    
    EXPECT_TRUE(os->isRunning());
}

TEST_F(OperatingSystemTest, KillNonExistentProcess) {
    os->boot();
    os->killProcess(999);  // Не существует
    EXPECT_TRUE(os->isRunning());
}

TEST_F(OperatingSystemTest, KillSystemProcess) {
    os->boot();  // PID 0: System
    os->killProcess(0);
    EXPECT_TRUE(os->isRunning());
}

TEST_F(OperatingSystemTest, KillMultipleProcesses) {
    os->boot();  // PID 0
    os->startProcess("Chrome", 500);  // PID 1
    os->startProcess("VSCode", 300);  // PID 2
    os->startProcess("Spotify", 200); // PID 3
    
    os->killProcess(1);
    os->killProcess(3);
    
    EXPECT_TRUE(os->isRunning());
}

TEST_F(OperatingSystemTest, KillProcessTwice) {
    os->boot();
    os->startProcess("Chrome", 500);  // PID 1
    
    os->killProcess(1);
    os->killProcess(1);  // Уже удален
    
    EXPECT_TRUE(os->isRunning());
}

TEST_F(OperatingSystemTest, KillAllProcesses) {
    os->boot();  // PID 0
    os->startProcess("P1", 100);  // PID 1
    os->startProcess("P2", 200);  // PID 2
    
    os->killProcess(0);
    os->killProcess(1);
    os->killProcess(2);
    
    EXPECT_TRUE(os->isRunning());
}

TEST_F(OperatingSystemTest, KillProcessNegativePID) {
    os->boot();
    os->killProcess(-1);
    EXPECT_TRUE(os->isRunning());
}

// ═══════════════════════════════════════════════════════════════════
// ИНТЕГРАЦИОННЫЕ ТЕСТЫ
// ═══════════════════════════════════════════════════════════════════

TEST_F(OperatingSystemTest, BootRunShutdownCycle) {
    os->boot();
    EXPECT_TRUE(os->isRunning());
    
    os->startProcess("App", 100);
    
    os->shutdown();
    EXPECT_FALSE(os->isRunning());
}

TEST_F(OperatingSystemTest, CompleteWorkflow) {
    os->boot();
    
    os->startProcess("Chrome", 500);
    os->startProcess("VSCode", 300);
    os->startProcess("Terminal", 100);
    
    os->killProcess(1);  // Kill Chrome
    
    os->startProcess("Firefox", 450);
    
    os->shutdown();
    EXPECT_FALSE(os->isRunning());
}

TEST_F(OperatingSystemTest, MultipleBootShutdown) {
    os->boot();
    os->shutdown();
    
    os->boot();
    os->shutdown();
    
    os->boot();
    EXPECT_TRUE(os->isRunning());
}

TEST_F(OperatingSystemTest, ProcessManagement) {
    os->boot();
    
    for (int i = 0; i < 10; i++) {
        os->startProcess("Process" + std::to_string(i), 100 * i);
    }
    
    os->killProcess(5);
    os->killProcess(7);
    os->killProcess(9);
    
    os->shutdown();
    EXPECT_FALSE(os->isRunning());
}

TEST_F(OperatingSystemTest, StartProcessesAfterShutdown) {
    os->boot();
    os->shutdown();
    
    os->startProcess("App", 100);
    
    EXPECT_FALSE(os->isRunning());
}

TEST_F(OperatingSystemTest, RapidProcessCreationAndTermination) {
    os->boot();
    
    for (int i = 0; i < 50; i++) {
        os->startProcess("Temp" + std::to_string(i), 50);
    }
    
    for (int i = 1; i <= 25; i++) {
        os->killProcess(i);
    }
    
    EXPECT_TRUE(os->isRunning());
}
