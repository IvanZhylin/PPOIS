#include <gtest/gtest.h>
#include "../../include/storage/StorageController.h"

class StorageControllerTest : public ::testing::Test {
protected:
    StorageController* controller;
    
    void SetUp() override {
        controller = new StorageController("SATA", 6);
    }
    
    void TearDown() override {
        delete controller;
    }
};

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ КОНСТРУКТОРА И ГЕТТЕРОВ
// ═══════════════════════════════════════════════════════════════════

TEST_F(StorageControllerTest, InitialState) {
    EXPECT_EQ(controller->getInterface(), "SATA");
    EXPECT_EQ(controller->getMaxDevices(), 6);
}

TEST_F(StorageControllerTest, SATAController) {
    StorageController* sata = new StorageController("SATA III", 8);
    EXPECT_EQ(sata->getInterface(), "SATA III");
    EXPECT_EQ(sata->getMaxDevices(), 8);
    delete sata;
}

TEST_F(StorageControllerTest, NVMeController) {
    StorageController* nvme = new StorageController("NVMe", 4);
    EXPECT_EQ(nvme->getInterface(), "NVMe");
    EXPECT_EQ(nvme->getMaxDevices(), 4);
    delete nvme;
}

TEST_F(StorageControllerTest, SASController) {
    StorageController* sas = new StorageController("SAS", 16);
    EXPECT_EQ(sas->getInterface(), "SAS");
    EXPECT_EQ(sas->getMaxDevices(), 16);
    delete sas;
}

TEST_F(StorageControllerTest, PCIeController) {
    StorageController* pcie = new StorageController("PCIe 4.0", 2);
    EXPECT_EQ(pcie->getInterface(), "PCIe 4.0");
    EXPECT_EQ(pcie->getMaxDevices(), 2);
    delete pcie;
}

TEST_F(StorageControllerTest, M2Controller) {
    StorageController* m2 = new StorageController("M.2", 3);
    EXPECT_EQ(m2->getInterface(), "M.2");
    EXPECT_EQ(m2->getMaxDevices(), 3);
    delete m2;
}

TEST_F(StorageControllerTest, ZeroDevices) {
    StorageController* zero = new StorageController("USB", 0);
    EXPECT_EQ(zero->getMaxDevices(), 0);
    delete zero;
}

TEST_F(StorageControllerTest, ManyDevices) {
    StorageController* many = new StorageController("SAS", 128);
    EXPECT_EQ(many->getMaxDevices(), 128);
    delete many;
}

TEST_F(StorageControllerTest, EmptyInterface) {
    StorageController* empty = new StorageController("", 4);
    EXPECT_EQ(empty->getInterface(), "");
    delete empty;
}

TEST_F(StorageControllerTest, LongInterfaceName) {
    std::string longName = "PCIe 5.0 x4 NVMe RAID Controller";
    StorageController* longCtrl = new StorageController(longName, 10);
    EXPECT_EQ(longCtrl->getInterface(), longName);
    delete longCtrl;
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ OPTIMIZE IO
// ═══════════════════════════════════════════════════════════════════

TEST_F(StorageControllerTest, OptimizeIO) {
    // Просто вызываем метод - он не падает
    controller->optimizeIO();
    
    // Проверяем что состояние не изменилось
    EXPECT_EQ(controller->getInterface(), "SATA");
    EXPECT_EQ(controller->getMaxDevices(), 6);
}

TEST_F(StorageControllerTest, OptimizeIOMultipleTimes) {
    controller->optimizeIO();
    controller->optimizeIO();
    controller->optimizeIO();
    
    EXPECT_EQ(controller->getInterface(), "SATA");
    EXPECT_EQ(controller->getMaxDevices(), 6);
}

// ═══════════════════════════════════════════════════════════════════
// ГРАНИЧНЫЕ СЛУЧАИ
// ═══════════════════════════════════════════════════════════════════

TEST_F(StorageControllerTest, NegativeDevices) {
    StorageController* neg = new StorageController("SATA", -1);
    EXPECT_EQ(neg->getMaxDevices(), -1);
    delete neg;
}

TEST_F(StorageControllerTest, SpecialCharactersInInterface) {
    StorageController* special = new StorageController("SATA-III_v2.0", 6);
    EXPECT_EQ(special->getInterface(), "SATA-III_v2.0");
    delete special;
}

// ═══════════════════════════════════════════════════════════════════
// ИНТЕГРАЦИОННЫЕ ТЕСТЫ
// ═══════════════════════════════════════════════════════════════════

TEST_F(StorageControllerTest, MultipleControllersIndependent) {
    StorageController* c1 = new StorageController("SATA", 6);
    StorageController* c2 = new StorageController("NVMe", 4);
    StorageController* c3 = new StorageController("SAS", 8);
    
    EXPECT_EQ(c1->getInterface(), "SATA");
    EXPECT_EQ(c2->getInterface(), "NVMe");
    EXPECT_EQ(c3->getInterface(), "SAS");
    
    EXPECT_EQ(c1->getMaxDevices(), 6);
    EXPECT_EQ(c2->getMaxDevices(), 4);
    EXPECT_EQ(c3->getMaxDevices(), 8);
    
    c1->optimizeIO();
    c2->optimizeIO();
    
    EXPECT_EQ(c1->getInterface(), "SATA");
    EXPECT_EQ(c2->getInterface(), "NVMe");
    
    delete c1;
    delete c2;
    delete c3;
}

TEST_F(StorageControllerTest, CopyInterface) {
    std::string iface = controller->getInterface();
    EXPECT_EQ(iface, "SATA");
    
    iface = "Modified";
    EXPECT_EQ(controller->getInterface(), "SATA");  // Не изменилось
}
