#include "../../include/motherboard/Motherboard.h"
Motherboard::Motherboard(const std::string& name, const std::string& manufacturer,
                         const std::string& socket, double price)
    : Component(name, manufacturer, price),
      formFactor("ATX"), socketType(socket),
      chipset("Z690", 10, 6, true),
      bios("F1.0"),
      memorySlots(4), m2Slots(2) {
    pcieSlots.emplace_back(1, "PCIe 5.0", 16);
    pcieSlots.emplace_back(2, "PCIe 4.0", 8);
    pcieSlots.emplace_back(3, "PCIe 3.0", 4);
}
void Motherboard::installComponent(int slotIndex) {
    if (slotIndex < (int)pcieSlots.size()) {
        pcieSlots[slotIndex].insertCard();
    }
}
void Motherboard::updateBIOS(const std::string& version) {
    bios.updateFirmware(version);
}
void Motherboard::configureBIOS(const std::string& setting, const std::string& value) {
    bios.changeSetting(setting, value);
}
std::string Motherboard::getSocketType() const { return socketType; }
int Motherboard::getMemorySlots() const { return memorySlots; }
std::string Motherboard::getDiagnostics() const {
    return Component::getDiagnostics() +
           " | Socket: " + socketType +
           " | BIOS: " + bios.getVersion();
}
