#ifndef RAM_H
#define RAM_H

#include "../interfaces/Interfaces.h"
#include "MemoryModule.h"
#include "MemoryController.h"
#include <vector>
#include <cstddef>

class RAM : public Component {
private:
    std::vector<MemoryModule> modules;
    MemoryController controller;
    size_t totalCapacityGB;
    size_t usedMemoryGB;

public:
    RAM(const std::string& name, const std::string& manufacturer,
        size_t capacity, int frequency, double price);

    void allocate(size_t sizeGB);
    void deallocate(size_t sizeGB);
    double getUsagePercent() const;
    void writeData(uint64_t address, uint8_t value);
    uint8_t readData(uint64_t address) const;
    void clearAll();

    size_t getTotalCapacity() const;
    size_t getUsedMemory() const;
    size_t getAvailableMemory() const;
    std::string getDiagnostics() const override;
};

#endif // RAM_H
