#include <gtest/gtest.h>
#include "../../include/peripherals/Mouse.h"

class MouseTest : public ::testing::Test {
protected:
    Mouse* mouse;
    
    void SetUp() override {
        mouse = new Mouse("Logitech G502", 16000, false);
    }
    
    void TearDown() override {
        delete mouse;
    }
};

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ КОНСТРУКТОРА И ГЕТТЕРОВ
// ═══════════════════════════════════════════════════════════════════

TEST_F(MouseTest, InitialState) {
    EXPECT_EQ(mouse->getName(), "Logitech G502");
    EXPECT_EQ(mouse->getDPI(), 16000);
}

TEST_F(MouseTest, WiredMouse) {
    Mouse* wired = new Mouse("Razer DeathAdder", 20000, false);
    EXPECT_EQ(wired->getDPI(), 20000);
    delete wired;
}

TEST_F(MouseTest, WirelessMouse) {
    Mouse* wireless = new Mouse("Logitech G Pro", 25600, true);
    EXPECT_EQ(wireless->getDPI(), 25600);
    delete wireless;
}

TEST_F(MouseTest, LowDPI) {
    Mouse* low = new Mouse("Basic Mouse", 800, false);
    EXPECT_EQ(low->getDPI(), 800);
    delete low;
}

TEST_F(MouseTest, HighDPI) {
    Mouse* high = new Mouse("Razer Viper", 30000, true);
    EXPECT_EQ(high->getDPI(), 30000);
    delete high;
}

TEST_F(MouseTest, ZeroDPI) {
    Mouse* zero = new Mouse("Broken", 0, false);
    EXPECT_EQ(zero->getDPI(), 0);
    delete zero;
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ CONNECT/DISCONNECT
// ═══════════════════════════════════════════════════════════════════

TEST_F(MouseTest, Connect) {
    mouse->connect();
    EXPECT_EQ(mouse->getDPI(), 16000);
}

TEST_F(MouseTest, Disconnect) {
    mouse->disconnect();
    EXPECT_EQ(mouse->getDPI(), 16000);
}

TEST_F(MouseTest, ConnectMultipleTimes) {
    mouse->connect();
    mouse->connect();
    mouse->connect();
    EXPECT_EQ(mouse->getDPI(), 16000);
}

TEST_F(MouseTest, DisconnectMultipleTimes) {
    mouse->disconnect();
    mouse->disconnect();
    mouse->disconnect();
    EXPECT_EQ(mouse->getDPI(), 16000);
}

TEST_F(MouseTest, ConnectAndDisconnect) {
    mouse->connect();
    mouse->disconnect();
    mouse->connect();
    mouse->disconnect();
    EXPECT_EQ(mouse->getDPI(), 16000);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ CLICK
// ═══════════════════════════════════════════════════════════════════

TEST_F(MouseTest, ClickLeftButton) {
    mouse->click(1);
    EXPECT_EQ(mouse->getDPI(), 16000);
}

TEST_F(MouseTest, ClickRightButton) {
    mouse->click(2);
    EXPECT_EQ(mouse->getDPI(), 16000);
}

TEST_F(MouseTest, ClickMiddleButton) {
    mouse->click(3);
    EXPECT_EQ(mouse->getDPI(), 16000);
}

TEST_F(MouseTest, ClickSideButtons) {
    mouse->click(4);
    mouse->click(5);
    EXPECT_EQ(mouse->getDPI(), 16000);
}

TEST_F(MouseTest, ClickInvalidButton) {
    mouse->click(0);
    mouse->click(-1);
    mouse->click(100);
    EXPECT_EQ(mouse->getDPI(), 16000);
}

TEST_F(MouseTest, ClickMultipleTimes) {
    for (int i = 0; i < 100; i++) {
        mouse->click(1);
    }
    EXPECT_EQ(mouse->getDPI(), 16000);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ SET DPI
// ═══════════════════════════════════════════════════════════════════

TEST_F(MouseTest, SetDPILow) {
    mouse->setDPI(400);
    EXPECT_EQ(mouse->getDPI(), 400);
}

TEST_F(MouseTest, SetDPIMedium) {
    mouse->setDPI(1600);
    EXPECT_EQ(mouse->getDPI(), 1600);
}

TEST_F(MouseTest, SetDPIHigh) {
    mouse->setDPI(12000);
    EXPECT_EQ(mouse->getDPI(), 12000);
}

TEST_F(MouseTest, SetDPIVeryHigh) {
    mouse->setDPI(30000);
    EXPECT_EQ(mouse->getDPI(), 30000);
}

TEST_F(MouseTest, SetDPIZero) {
    mouse->setDPI(0);
    EXPECT_EQ(mouse->getDPI(), 0);
}

TEST_F(MouseTest, SetDPINegative) {
    mouse->setDPI(-100);
    EXPECT_EQ(mouse->getDPI(), -100);
}

TEST_F(MouseTest, SetDPIMultipleTimes) {
    mouse->setDPI(800);
    EXPECT_EQ(mouse->getDPI(), 800);
    
    mouse->setDPI(1600);
    EXPECT_EQ(mouse->getDPI(), 1600);
    
    mouse->setDPI(3200);
    EXPECT_EQ(mouse->getDPI(), 3200);
}

TEST_F(MouseTest, SetDPISteps) {
    int dpiSteps[] = {400, 800, 1600, 3200, 6400, 12800, 25600};
    
    for (int dpi : dpiSteps) {
        mouse->setDPI(dpi);
        EXPECT_EQ(mouse->getDPI(), dpi);
    }
}

// ═══════════════════════════════════════════════════════════════════
// ИНТЕГРАЦИОННЫЕ ТЕСТЫ
// ═══════════════════════════════════════════════════════════════════

TEST_F(MouseTest, CompleteUsage) {
    mouse->connect();
    mouse->setDPI(800);
    mouse->click(1);
    mouse->click(2);
    mouse->setDPI(1600);
    mouse->click(1);
    mouse->disconnect();
    
    EXPECT_EQ(mouse->getDPI(), 1600);
}

TEST_F(MouseTest, GamingSession) {
    mouse->connect();
    mouse->setDPI(400);   // Precision aim
    
    for (int i = 0; i < 50; i++) {
        mouse->click(1);
    }
    
    mouse->setDPI(1600);  // Normal aim
    
    for (int i = 0; i < 30; i++) {
        mouse->click(1);
    }
    
    mouse->disconnect();
    EXPECT_EQ(mouse->getDPI(), 1600);
}

TEST_F(MouseTest, DPICycling) {
    mouse->setDPI(400);
    mouse->click(4);  // DPI button
    mouse->setDPI(800);
    mouse->click(4);
    mouse->setDPI(1600);
    mouse->click(4);
    mouse->setDPI(3200);
    
    EXPECT_EQ(mouse->getDPI(), 3200);
}
