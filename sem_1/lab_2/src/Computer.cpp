#include "../include/Computer.h"
#include "../include/exceptions/Exceptions.h"
#include "../include/network/NetworkPacket.h"
#include <iostream>

Computer::Computer(const std::string& name) 
    : computerName(name), systemOn(false), totalPowerConsumption(0) {}

Computer::~Computer() {
    if (systemOn) {
        powerOff();
    }
}

void Computer::assembleParts() {
    cpu = std::make_unique<CPU>("Ryzen 9 5900X", "AMD", 12, 3.7, 549.99);
    ram = std::make_unique<RAM>("Vengeance RGB", "Corsair", 32, 3600, 159.99);
    storage = std::make_unique<HardDrive>("970 EVO", "Samsung", 1000, true, 149.99);
    gpu = std::make_unique<GraphicsCard>("RTX 4080", "NVIDIA", 9728, 2.5, 16, 1199.99);
    motherboard = std::make_unique<Motherboard>("X570 AORUS", "Gigabyte", "AM4", 299.99);
    psu = std::make_unique<PowerSupply>("RM850x", "Corsair", 850, "80+ Gold", 129.99);
    cooler = std::make_unique<CPUCooler>("Kraken X63", "NZXT", "AIO", 280, 149.99);
    network = std::make_unique<NetworkAdapter>("Intel I225-V", "Intel", 2500, false, 0);
    soundCard = std::make_unique<SoundCard>("AE-9", "Creative", 8, 349.99);
    chassisCase = std::make_unique<ComputerCase>("H710", "NZXT", "ATX", 169.99);
    display = std::make_unique<Monitor>("PG279Q", "ASUS", 2560, 1440, 165, 699.99);
    keyboard = std::make_unique<Keyboard>("K95 RGB", true, true);
    mouse = std::make_unique<Mouse>("G502", 16000, false);
    os = std::make_unique<OperatingSystem>("Windows", "11");
}

void Computer::powerOn() {
    if (systemOn) return;

    try {
        psu->powerOn();
        psu->supplyPower(450.0);

        motherboard->powerOn();
        cpu->powerOn();
        ram->powerOn();
        storage->powerOn();
        gpu->powerOn();
        network->powerOn();
        soundCard->powerOn();
        display->powerOn();

        os->boot();

        systemOn = true;
        totalPowerConsumption = 450.0;

    } catch (const HardwareException& e) {
        std::cerr << "Hardware error: " << e.what() << std::endl;
        powerOff();
    }
}

void Computer::powerOff() {
    if (!systemOn) return;

    os->shutdown();

    display->powerOff();
    soundCard->powerOff();
    network->powerOff();
    gpu->powerOff();
    storage->powerOff();
    ram->powerOff();
    cpu->powerOff();
    motherboard->powerOff();
    psu->powerOff();

    systemOn = false;
    totalPowerConsumption = 0;
}

void Computer::restart() {
    powerOff();
    powerOn();
}

void Computer::performCPUTask() {
    if (!systemOn) throw std::runtime_error("Computer is off");

    try {
        cpu->processInstruction("ADD");
        cpu->processInstruction("MUL");
        cooler->coolComponent(*cpu);

    } catch (const CPUOverheatException& e) {
        std::cerr << "CPU overheat! Emergency shutdown." << std::endl;
        powerOff();
    }
}

void Computer::renderGraphics() {
    if (!systemOn) throw std::runtime_error("Computer is off");

    try {
        gpu->render3DScene();
        display->displayImage();

    } catch (const HardwareException& e) {
        std::cerr << "Graphics error: " << e.what() << std::endl;
    }
}

void Computer::allocateMemory(size_t sizeGB) {
    if (!systemOn) throw std::runtime_error("Computer is off");

    try {
        ram->allocate(sizeGB);
    } catch (const MemoryOverflowException& e) {
        std::cerr << "Memory overflow: " << e.what() << std::endl;
    }
}

void Computer::saveToStorage(const std::string& data) {
    if (!systemOn) throw std::runtime_error("Computer is off");

    try {
        storage->write(data);
    } catch (const DiskFullException& e) {
        std::cerr << "Disk full: " << e.what() << std::endl;
    }
}

void Computer::connectToNetwork() {
    if (!systemOn) throw std::runtime_error("Computer is off");

    try {
        network->powerOn();
        NetworkPacket packet("192.168.1.100", "8.8.8.8", "PING");
        network->sendPacket(packet);
    } catch (const NetworkConnectionException& e) {
        std::cerr << "Network error: " << e.what() << std::endl;
    }
}

std::string Computer::getSystemInfo() {
    std::string info = "=== " + computerName + " ===\n";
    info += cpu->getDiagnostics() + "\n";
    info += ram->getDiagnostics() + "\n";
    info += storage->getDiagnostics() + "\n";
    info += gpu->getDiagnostics() + "\n";
    info += motherboard->getDiagnostics() + "\n";
    info += psu->getDiagnostics() + "\n";
    info += "Status: " + std::string(systemOn ? "ON" : "OFF") + "\n";
    info += "Power: " + std::to_string(totalPowerConsumption) + "W\n";
    return info;
}

void Computer::runDiagnostics() {
    std::cout << "Running system diagnostics...\n";
    std::cout << getSystemInfo() << std::endl;

    double cpuTemp = cpu->getTemperature();
    double gpuTemp = gpu->getTemperature();

    std::cout << "CPU Temperature: " << cpuTemp << "C\n";
    std::cout << "GPU Temperature: " << gpuTemp << "C\n";
    std::cout << "RAM Usage: " << ram->getUsagePercent() << "%\n";
    std::cout << "PSU Load: " << psu->getLoadPercent() << "%\n";
}

bool Computer::isOn() const {
    return systemOn;
}

std::string Computer::getName() const {
    return computerName;
}
