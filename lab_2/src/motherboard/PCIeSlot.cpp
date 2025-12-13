#include "../../include/motherboard/PCIeSlot.h"
#include <stdexcept>
PCIeSlot::PCIeSlot(int num, const std::string& gen, int l)
    : slotNumber(num), generation(gen), lanes(l), occupied(false) {}
void PCIeSlot::insertCard() {
    if (occupied) throw std::runtime_error("Slot occupied");
    occupied = true;
}
void PCIeSlot::removeCard() { occupied = false; }
bool PCIeSlot::isOccupied() const { return occupied; }
int PCIeSlot::getLanes() const { return lanes; }
