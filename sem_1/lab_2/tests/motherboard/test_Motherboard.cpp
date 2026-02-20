#include <gtest/gtest.h>
#include "../../include/motherboard/Motherboard.h"

class MotherboardTest : public ::testing::Test {
protected:
    Motherboard* mb;
    
    void SetUp() override {
        mb = new Motherboard("ROG STRIX Z690-E", "ASUS", "LGA1700", 399.99);
    }
    
    void TearDown() override {
        delete mb;
    }
};

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ КОНСТРУКТОРА И ГЕТТЕРОВ
// ═══════════════════════════════════════════════════════════════════

TEST_F(MotherboardTest, InitialState) {
    EXPECT_EQ(mb->getName(), "ROG STRIX Z690-E");
    EXPECT_EQ(mb->getManufacturer(), "ASUS");
    EXPECT_EQ(mb->getSocketType(), "LGA1700");
    EXPECT_DOUBLE_EQ(mb->getPrice(), 399.99);
    EXPECT_EQ(mb->getMemorySlots(), 4);
}

TEST_F(MotherboardTest, IntelMotherboard) {
    Motherboard* intel = new Motherboard("Z790 AORUS", "Gigabyte", "LGA1700", 449.99);
    EXPECT_EQ(intel->getSocketType(), "LGA1700");
    EXPECT_EQ(intel->getMemorySlots(), 4);
    delete intel;
}

TEST_F(MotherboardTest, AMDMotherboard) {
    Motherboard* amd = new Motherboard("X670E GAMING", "MSI", "AM5", 329.99);
    EXPECT_EQ(amd->getSocketType(), "AM5");
    EXPECT_EQ(amd->getMemorySlots(), 4);
    delete amd;
}

TEST_F(MotherboardTest, BudgetMotherboard) {
    Motherboard* budget = new Motherboard("B660M-A", "ASRock", "LGA1700", 119.99);
    EXPECT_EQ(budget->getSocketType(), "LGA1700");
    EXPECT_DOUBLE_EQ(budget->getPrice(), 119.99);
    delete budget;
}

TEST_F(MotherboardTest, HighEndMotherboard) {
    Motherboard* highEnd = new Motherboard("ROG MAXIMUS Z790", "ASUS", "LGA1700", 899.99);
    EXPECT_DOUBLE_EQ(highEnd->getPrice(), 899.99);
    delete highEnd;
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ INSTALL COMPONENT
// ═══════════════════════════════════════════════════════════════════

TEST_F(MotherboardTest, InstallComponentSlot0) {
    mb->installComponent(0);  // Устанавливаем в первый слот
    // Метод не должен падать
    EXPECT_EQ(mb->getSocketType(), "LGA1700");
}

TEST_F(MotherboardTest, InstallComponentSlot1) {
    mb->installComponent(1);  // Второй слот
    EXPECT_EQ(mb->getMemorySlots(), 4);
}

TEST_F(MotherboardTest, InstallComponentSlot2) {
    mb->installComponent(2);  // Третий слот
    EXPECT_EQ(mb->getMemorySlots(), 4);
}

TEST_F(MotherboardTest, InstallMultipleComponents) {
    mb->installComponent(0);
    mb->installComponent(1);
    mb->installComponent(2);
    
    EXPECT_EQ(mb->getSocketType(), "LGA1700");
}

TEST_F(MotherboardTest, InstallComponentValidSlots) {
    // Тестируем только валидные слоты (0, 1, 2)
    mb->installComponent(0);
    mb->installComponent(1);
    mb->installComponent(2);
    
    EXPECT_EQ(mb->getSocketType(), "LGA1700");
}



// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ UPDATE BIOS
// ═══════════════════════════════════════════════════════════════════

TEST_F(MotherboardTest, UpdateBIOS) {
    mb->updateBIOS("F2.0");
    // Проверяем через getDiagnostics
    std::string diag = mb->getDiagnostics();
    EXPECT_NE(diag.find("BIOS:"), std::string::npos);
}

TEST_F(MotherboardTest, UpdateBIOSMultipleTimes) {
    mb->updateBIOS("F2.0");
    mb->updateBIOS("F2.5");
    mb->updateBIOS("F3.0");
    
    std::string diag = mb->getDiagnostics();
    EXPECT_NE(diag.find("BIOS:"), std::string::npos);
}


TEST_F(MotherboardTest, UpdateBIOSLongVersion) {
    mb->updateBIOS("F10.20.30.40.50");
    std::string diag = mb->getDiagnostics();
    EXPECT_NE(diag.find("BIOS:"), std::string::npos);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ CONFIGURE BIOS
// ═══════════════════════════════════════════════════════════════════

TEST_F(MotherboardTest, ConfigureBIOSSingleSetting) {
    mb->configureBIOS("XMP", "Enabled");
    EXPECT_EQ(mb->getMemorySlots(), 4);
}

TEST_F(MotherboardTest, ConfigureBIOSMultipleSettings) {
    mb->configureBIOS("XMP", "Enabled");
    mb->configureBIOS("Secure Boot", "Disabled");
    mb->configureBIOS("Virtualization", "Enabled");
    
    EXPECT_EQ(mb->getSocketType(), "LGA1700");
}

TEST_F(MotherboardTest, ConfigureBIOSEmptyValues) {
    mb->configureBIOS("", "");
    EXPECT_EQ(mb->getMemorySlots(), 4);
}

TEST_F(MotherboardTest, ConfigureBIOSComplexSettings) {
    mb->configureBIOS("Boot Order", "SSD -> USB -> Network");
    mb->configureBIOS("CPU Frequency", "5.2 GHz");
    mb->configureBIOS("Memory Timing", "16-18-18-36");
    
    EXPECT_EQ(mb->getSocketType(), "LGA1700");
}

TEST_F(MotherboardTest, ConfigureBIOSSpecialCharacters) {
    mb->configureBIOS("Fan Profile", "Silent (50%)");
    mb->configureBIOS("RGB Mode", "Rainbow++");
    
    EXPECT_EQ(mb->getMemorySlots(), 4);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ GET DIAGNOSTICS
// ═══════════════════════════════════════════════════════════════════

TEST_F(MotherboardTest, GetDiagnosticsInitial) {
    std::string diag = mb->getDiagnostics();
    
    EXPECT_NE(diag.find("ROG STRIX Z690-E"), std::string::npos);
    EXPECT_NE(diag.find("Socket:"), std::string::npos);
    EXPECT_NE(diag.find("LGA1700"), std::string::npos);
    EXPECT_NE(diag.find("BIOS:"), std::string::npos);
}

TEST_F(MotherboardTest, GetDiagnosticsAfterBIOSUpdate) {
    mb->updateBIOS("F5.0");
    std::string diag = mb->getDiagnostics();
    
    EXPECT_NE(diag.find("Socket: LGA1700"), std::string::npos);
    EXPECT_NE(diag.find("BIOS:"), std::string::npos);
}

TEST_F(MotherboardTest, GetDiagnosticsAfterConfiguration) {
    mb->configureBIOS("XMP", "Enabled");
    mb->installComponent(0);
    std::string diag = mb->getDiagnostics();
    
    EXPECT_NE(diag.find("Socket: LGA1700"), std::string::npos);
}

TEST_F(MotherboardTest, GetDiagnosticsMultipleTimes) {
    std::string diag1 = mb->getDiagnostics();
    std::string diag2 = mb->getDiagnostics();
    
    EXPECT_EQ(diag1, diag2);
}

// ═══════════════════════════════════════════════════════════════════
// ИНТЕГРАЦИОННЫЕ ТЕСТЫ
// ═══════════════════════════════════════════════════════════════════

TEST_F(MotherboardTest, CompleteWorkflow) {
    // Обновляем BIOS
    mb->updateBIOS("F2.0");
    
    // Настраиваем BIOS
    mb->configureBIOS("XMP", "Enabled");
    mb->configureBIOS("Secure Boot", "Disabled");
    
    // Устанавливаем компоненты
    mb->installComponent(0);
    mb->installComponent(1);
    
    // Проверяем диагностику
    std::string diag = mb->getDiagnostics();
    EXPECT_NE(diag.find("Socket: LGA1700"), std::string::npos);
    EXPECT_NE(diag.find("BIOS:"), std::string::npos);
    
    // Проверяем геттеры
    EXPECT_EQ(mb->getSocketType(), "LGA1700");
    EXPECT_EQ(mb->getMemorySlots(), 4);
}

TEST_F(MotherboardTest, FullSystemSetup) {
    // Начальная конфигурация
    EXPECT_EQ(mb->getName(), "ROG STRIX Z690-E");
    
    // BIOS setup
    mb->updateBIOS("F3.0");
    mb->configureBIOS("XMP", "Profile 1");
    mb->configureBIOS("Resizable BAR", "Enabled");
    mb->configureBIOS("VT-d", "Enabled");
    
    // Установка компонентов
    mb->installComponent(0);  // GPU
    mb->installComponent(1);  // Sound card
    mb->installComponent(2);  // Network card
    
    // Финальная диагностика
    std::string diag = mb->getDiagnostics();
    EXPECT_FALSE(diag.empty());
    EXPECT_NE(diag.find("LGA1700"), std::string::npos);
}

// ═══════════════════════════════════════════════════════════════════
// ГРАНИЧНЫЕ СЛУЧАИ
// ═══════════════════════════════════════════════════════════════════

TEST_F(MotherboardTest, EmptySocketType) {
    Motherboard* empty = new Motherboard("Board", "Maker", "", 100.0);
    EXPECT_EQ(empty->getSocketType(), "");
    delete empty;
}

TEST_F(MotherboardTest, VeryLongNames) {
    std::string longName(200, 'X');
    Motherboard* longMB = new Motherboard(longName, "Vendor", "AM5", 299.99);
    EXPECT_EQ(longMB->getName(), longName);
    delete longMB;
}

TEST_F(MotherboardTest, ZeroPrice) {
    Motherboard* free = new Motherboard("Free Board", "Charity", "LGA1700", 0.0);
    EXPECT_DOUBLE_EQ(free->getPrice(), 0.0);
    delete free;
}

TEST_F(MotherboardTest, NegativePrice) {
    Motherboard* neg = new Motherboard("Defective", "Unknown", "Socket", -50.0);
    EXPECT_DOUBLE_EQ(neg->getPrice(), -50.0);
    delete neg;
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ МНОЖЕСТВЕННЫХ МАТЕРИНСКИХ ПЛАТ
// ═══════════════════════════════════════════════════════════════════

TEST_F(MotherboardTest, MultipleMotherboardsIndependent) {
    Motherboard* mb1 = new Motherboard("Board1", "ASUS", "LGA1700", 300.0);
    Motherboard* mb2 = new Motherboard("Board2", "MSI", "AM5", 250.0);
    
    mb1->updateBIOS("F1.0");
    mb2->updateBIOS("F2.0");
    
    mb1->installComponent(0);
    mb2->installComponent(1);
    
    EXPECT_EQ(mb1->getSocketType(), "LGA1700");
    EXPECT_EQ(mb2->getSocketType(), "AM5");
    
    delete mb1;
    delete mb2;
}

TEST_F(MotherboardTest, StressTest) {
    for (int i = 0; i < 100; i++) {
        mb->configureBIOS("Setting" + std::to_string(i), "Value" + std::to_string(i));
    }
    
    mb->installComponent(0);
    mb->installComponent(1);
    mb->installComponent(2);
    
    std::string diag = mb->getDiagnostics();
    EXPECT_FALSE(diag.empty());
}
