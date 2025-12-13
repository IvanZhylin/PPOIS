#include <gtest/gtest.h>
#include "../../include/peripherals/Monitor.h"

class MonitorTest : public ::testing::Test {
protected:
    Monitor* monitor;
    
    void SetUp() override {
        monitor = new Monitor("LG 27GL850", "LG", 2560, 1440, 144, 499.99);
    }
    
    void TearDown() override {
        delete monitor;
    }
};

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ КОНСТРУКТОРА И ГЕТТЕРОВ
// ═══════════════════════════════════════════════════════════════════

TEST_F(MonitorTest, InitialState) {
    EXPECT_EQ(monitor->getName(), "LG 27GL850");
    EXPECT_EQ(monitor->getManufacturer(), "LG");
    EXPECT_EQ(monitor->getWidth(), 2560);
    EXPECT_EQ(monitor->getHeight(), 1440);
    EXPECT_DOUBLE_EQ(monitor->getPrice(), 499.99);
    EXPECT_FALSE(monitor->isPowered());
}

TEST_F(MonitorTest, FullHD60Hz) {
    Monitor* fhd = new Monitor("Basic Monitor", "Dell", 1920, 1080, 60, 199.99);
    EXPECT_EQ(fhd->getWidth(), 1920);
    EXPECT_EQ(fhd->getHeight(), 1080);
    delete fhd;
}

TEST_F(MonitorTest, QHD144Hz) {
    Monitor* qhd = new Monitor("Gaming Monitor", "ASUS", 2560, 1440, 144, 399.99);
    EXPECT_EQ(qhd->getWidth(), 2560);
    EXPECT_EQ(qhd->getHeight(), 1440);
    delete qhd;
}

TEST_F(MonitorTest, UHD60Hz) {
    Monitor* uhd = new Monitor("4K Monitor", "Samsung", 3840, 2160, 60, 599.99);
    EXPECT_EQ(uhd->getWidth(), 3840);
    EXPECT_EQ(uhd->getHeight(), 2160);
    delete uhd;
}

TEST_F(MonitorTest, UltraWide) {
    Monitor* ultrawide = new Monitor("Curved", "LG", 3440, 1440, 100, 799.99);
    EXPECT_EQ(ultrawide->getWidth(), 3440);
    EXPECT_EQ(ultrawide->getHeight(), 1440);
    delete ultrawide;
}

TEST_F(MonitorTest, HighRefreshRate) {
    Monitor* fast = new Monitor("Pro Gaming", "Alienware", 1920, 1080, 360, 699.99);
    EXPECT_EQ(fast->getWidth(), 1920);
    EXPECT_EQ(fast->getHeight(), 1080);
    delete fast;
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ DISPLAY IMAGE
// ═══════════════════════════════════════════════════════════════════

TEST_F(MonitorTest, DisplayImageWhenOff) {
    EXPECT_THROW(monitor->displayImage(), std::runtime_error);
}

TEST_F(MonitorTest, DisplayImageWhenOn) {
    monitor->powerOn();
    EXPECT_NO_THROW(monitor->displayImage());
}

TEST_F(MonitorTest, DisplayImageMultipleTimes) {
    monitor->powerOn();
    monitor->displayImage();
    monitor->displayImage();
    monitor->displayImage();
    EXPECT_TRUE(monitor->isPowered());
}

TEST_F(MonitorTest, DisplayImageAfterPowerOff) {
    monitor->powerOn();
    monitor->displayImage();  // OK
    monitor->powerOff();
    EXPECT_THROW(monitor->displayImage(), std::runtime_error);
}

TEST_F(MonitorTest, DisplayImageAfterPowerCycle) {
    monitor->powerOn();
    monitor->displayImage();
    monitor->powerOff();
    monitor->powerOn();
    EXPECT_NO_THROW(monitor->displayImage());
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ CHANGE RESOLUTION
// ═══════════════════════════════════════════════════════════════════

TEST_F(MonitorTest, ChangeResolutionToFullHD) {
    monitor->changeResolution(1920, 1080);
    EXPECT_EQ(monitor->getWidth(), 1920);
    EXPECT_EQ(monitor->getHeight(), 1080);
}

TEST_F(MonitorTest, ChangeResolutionToQHD) {
    monitor->changeResolution(2560, 1440);
    EXPECT_EQ(monitor->getWidth(), 2560);
    EXPECT_EQ(monitor->getHeight(), 1440);
}

TEST_F(MonitorTest, ChangeResolutionTo4K) {
    monitor->changeResolution(3840, 2160);
    EXPECT_EQ(monitor->getWidth(), 3840);
    EXPECT_EQ(monitor->getHeight(), 2160);
}

TEST_F(MonitorTest, ChangeResolutionTo8K) {
    monitor->changeResolution(7680, 4320);
    EXPECT_EQ(monitor->getWidth(), 7680);
    EXPECT_EQ(monitor->getHeight(), 4320);
}

TEST_F(MonitorTest, ChangeResolutionToLow) {
    monitor->changeResolution(800, 600);
    EXPECT_EQ(monitor->getWidth(), 800);
    EXPECT_EQ(monitor->getHeight(), 600);
}

TEST_F(MonitorTest, ChangeResolutionMultipleTimes) {
    monitor->changeResolution(1920, 1080);
    EXPECT_EQ(monitor->getWidth(), 1920);
    EXPECT_EQ(monitor->getHeight(), 1080);
    
    monitor->changeResolution(2560, 1440);
    EXPECT_EQ(monitor->getWidth(), 2560);
    EXPECT_EQ(monitor->getHeight(), 1440);
    
    monitor->changeResolution(3840, 2160);
    EXPECT_EQ(monitor->getWidth(), 3840);
    EXPECT_EQ(monitor->getHeight(), 2160);
}

TEST_F(MonitorTest, ChangeResolutionZero) {
    monitor->changeResolution(0, 0);
    EXPECT_EQ(monitor->getWidth(), 0);
    EXPECT_EQ(monitor->getHeight(), 0);
}

TEST_F(MonitorTest, ChangeResolutionNegative) {
    monitor->changeResolution(-1, -1);
    EXPECT_EQ(monitor->getWidth(), -1);
    EXPECT_EQ(monitor->getHeight(), -1);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ CHANGE REFRESH RATE
// ═══════════════════════════════════════════════════════════════════

TEST_F(MonitorTest, ChangeRefreshRateTo60) {
    monitor->changeRefreshRate(60);
    std::string diag = monitor->getDiagnostics();
    EXPECT_NE(diag.find("60Hz"), std::string::npos);
}

TEST_F(MonitorTest, ChangeRefreshRateTo144) {
    monitor->changeRefreshRate(144);
    std::string diag = monitor->getDiagnostics();
    EXPECT_NE(diag.find("144Hz"), std::string::npos);
}

TEST_F(MonitorTest, ChangeRefreshRateTo240) {
    monitor->changeRefreshRate(240);
    std::string diag = monitor->getDiagnostics();
    EXPECT_NE(diag.find("240Hz"), std::string::npos);
}

TEST_F(MonitorTest, ChangeRefreshRateMultipleTimes) {
    monitor->changeRefreshRate(60);
    monitor->changeRefreshRate(120);
    monitor->changeRefreshRate(144);
    monitor->changeRefreshRate(240);
    
    std::string diag = monitor->getDiagnostics();
    EXPECT_NE(diag.find("240Hz"), std::string::npos);
}

TEST_F(MonitorTest, ChangeRefreshRateZero) {
    monitor->changeRefreshRate(0);
    std::string diag = monitor->getDiagnostics();
    EXPECT_NE(diag.find("0Hz"), std::string::npos);
}

TEST_F(MonitorTest, ChangeRefreshRateNegative) {
    monitor->changeRefreshRate(-1);
    std::string diag = monitor->getDiagnostics();
    EXPECT_NE(diag.find("-1Hz"), std::string::npos);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ GET DIAGNOSTICS
// ═══════════════════════════════════════════════════════════════════

TEST_F(MonitorTest, GetDiagnostics) {
    std::string diag = monitor->getDiagnostics();
    
    EXPECT_NE(diag.find("LG 27GL850"), std::string::npos);
    EXPECT_NE(diag.find("2560x1440"), std::string::npos);
    EXPECT_NE(diag.find("144Hz"), std::string::npos);
}

TEST_F(MonitorTest, GetDiagnosticsAfterResolutionChange) {
    monitor->changeResolution(1920, 1080);
    std::string diag = monitor->getDiagnostics();
    
    EXPECT_NE(diag.find("1920x1080"), std::string::npos);
}

TEST_F(MonitorTest, GetDiagnosticsAfterRefreshRateChange) {
    monitor->changeRefreshRate(240);
    std::string diag = monitor->getDiagnostics();
    
    EXPECT_NE(diag.find("240Hz"), std::string::npos);
}

TEST_F(MonitorTest, GetDiagnosticsAfterBothChanges) {
    monitor->changeResolution(3840, 2160);
    monitor->changeRefreshRate(120);
    
    std::string diag = monitor->getDiagnostics();
    EXPECT_NE(diag.find("3840x2160@120Hz"), std::string::npos);
}

// ═══════════════════════════════════════════════════════════════════
// ИНТЕГРАЦИОННЫЕ ТЕСТЫ
// ═══════════════════════════════════════════════════════════════════

TEST_F(MonitorTest, CompleteWorkflow) {
    // Включение
    monitor->powerOn();
    EXPECT_TRUE(monitor->isPowered());
    
    // Отображение
    monitor->displayImage();
    
    // Смена разрешения
    monitor->changeResolution(1920, 1080);
    EXPECT_EQ(monitor->getWidth(), 1920);
    EXPECT_EQ(monitor->getHeight(), 1080);
    
    // Смена частоты
    monitor->changeRefreshRate(60);
    
    // Диагностика
    std::string diag = monitor->getDiagnostics();
    EXPECT_NE(diag.find("1920x1080@60Hz"), std::string::npos);
    
    // Выключение
    monitor->powerOff();
    EXPECT_FALSE(monitor->isPowered());
}

TEST_F(MonitorTest, GamingSetup) {
    monitor->powerOn();
    
    // Настройка для игр
    monitor->changeResolution(1920, 1080);
    monitor->changeRefreshRate(240);
    
    monitor->displayImage();
    
    std::string diag = monitor->getDiagnostics();
    EXPECT_NE(diag.find("1920x1080@240Hz"), std::string::npos);
}

TEST_F(MonitorTest, ProductivitySetup) {
    monitor->powerOn();
    
    // Настройка для работы
    monitor->changeResolution(3840, 2160);
    monitor->changeRefreshRate(60);
    
    monitor->displayImage();
    
    EXPECT_EQ(monitor->getWidth(), 3840);
    EXPECT_EQ(monitor->getHeight(), 2160);
}

TEST_F(MonitorTest, DynamicResolutionSwitching) {
    monitor->powerOn();
    
    // Игра
    monitor->changeResolution(1920, 1080);
    monitor->changeRefreshRate(144);
    monitor->displayImage();
    
    // Работа
    monitor->changeResolution(2560, 1440);
    monitor->changeRefreshRate(60);
    monitor->displayImage();
    
    // Видео 4K
    monitor->changeResolution(3840, 2160);
    monitor->changeRefreshRate(60);
    monitor->displayImage();
    
    EXPECT_EQ(monitor->getWidth(), 3840);
    EXPECT_EQ(monitor->getHeight(), 2160);
}
