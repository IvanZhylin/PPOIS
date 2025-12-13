#ifndef MOTHERBOARD_H
#define MOTHERBOARD_H
#include "../interfaces/Interfaces.h"
#include "PCIeSlot.h"
#include "Chipset.h"
#include "BIOS.h"
#include <vector>
class Motherboard : public Component {
private:
    std::string formFactor;
    std::string socketType;
    std::vector<PCIeSlot> pcieSlots;
    Chipset chipset;
    BIOS bios;
    int memorySlots;
    int m2Slots;
public:
    Motherboard(const std::string& name, const std::string& manufacturer,
                const std::string& socket, double price);
    void installComponent(int slotIndex);
    void updateBIOS(const std::string& version);
    void configureBIOS(const std::string& setting, const std::string& value);
    std::string getSocketType() const;
    int getMemorySlots() const;
    std::string getDiagnostics() const override;
};
#endif
