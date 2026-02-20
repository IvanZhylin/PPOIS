#ifndef PCIE_SLOT_H
#define PCIE_SLOT_H
#include <string>
class PCIeSlot {
private:
    int slotNumber;
    std::string generation;
    int lanes;
    bool occupied;
public:
    PCIeSlot(int num, const std::string& gen, int l);
    void insertCard();
    void removeCard();
    bool isOccupied() const;
    int getLanes() const;
};
#endif
