#ifndef COMPUTER_H
#define COMPUTER_H

#include "cpu/CPU.h"
#include "memory/RAM.h"
#include "storage/HardDrive.h"
#include "graphics/GraphicsCard.h"
#include "motherboard/Motherboard.h"
#include "power/PowerSupply.h"
#include "cooling/CPUCooler.h"
#include "network/NetworkAdapter.h"
#include "audio/SoundCard.h"
#include "case/ComputerCase.h"
#include "peripherals/Monitor.h"
#include "peripherals/Keyboard.h"
#include "peripherals/Mouse.h"
#include "os/OperatingSystem.h"
#include <memory>
#include <string>
#include <cstddef>

class Computer {
private:
    std::string computerName;
    std::unique_ptr<CPU> cpu;
    std::unique_ptr<RAM> ram;
    std::unique_ptr<HardDrive> storage;
    std::unique_ptr<GraphicsCard> gpu;
    std::unique_ptr<Motherboard> motherboard;
    std::unique_ptr<PowerSupply> psu;
    std::unique_ptr<CPUCooler> cooler;
    std::unique_ptr<NetworkAdapter> network;
    std::unique_ptr<SoundCard> soundCard;
    std::unique_ptr<ComputerCase> chassisCase;
    std::unique_ptr<Monitor> display;
    std::unique_ptr<Keyboard> keyboard;
    std::unique_ptr<Mouse> mouse;
    std::unique_ptr<OperatingSystem> os;
    bool systemOn;
    double totalPowerConsumption;

public:
    Computer(const std::string& name);
    ~Computer();

    void assembleParts();
    void powerOn();
    void powerOff();
    void restart();
    void performCPUTask();
    void renderGraphics();
    void allocateMemory(size_t sizeGB);
    void saveToStorage(const std::string& data);
    void connectToNetwork();
    std::string getSystemInfo();
    void runDiagnostics();

    bool isOn() const;
    std::string getName() const;
};

#endif // COMPUTER_H
