#include "../../include/memory/RAM.h"
#include "../../include/exceptions/Exceptions.h"

RAM::RAM(const std::string& name, const std::string& manufacturer,
         size_t capacity, int frequency, double price)
    : Component(name, manufacturer, price),
      controller(2, 25.6),
      totalCapacityGB(capacity),
      usedMemoryGB(0) {

    modules.emplace_back(capacity / 2, frequency, "DDR4");
    modules.emplace_back(capacity / 2, frequency, "DDR4");
}

void RAM::allocate(size_t sizeGB) {
    if (usedMemoryGB + sizeGB > totalCapacityGB) {
        throw MemoryOverflowException();
    }
    usedMemoryGB += sizeGB;
}

void RAM::deallocate(size_t sizeGB) {
    usedMemoryGB = (sizeGB > usedMemoryGB) ? 0 : usedMemoryGB - sizeGB;
}

double RAM::getUsagePercent() const {
    return (double)usedMemoryGB / totalCapacityGB * 100.0;
}

void RAM::writeData(uint64_t address, uint8_t value) {
    size_t moduleIndex = address / (totalCapacityGB * 1024 * 1024 * 1024 / 2);
    if (moduleIndex < modules.size()) {
        modules[moduleIndex].write(address, value);
    }
}

uint8_t RAM::readData(uint64_t address) const {
    size_t moduleIndex = address / (totalCapacityGB * 1024 * 1024 * 1024 / 2);
    if (moduleIndex < modules.size()) {
        return modules[moduleIndex].read(address);
    }
    return 0;
}

void RAM::clearAll() {
    for (auto& module : modules) {
        module.clear();
    }
    usedMemoryGB = 0;
}

size_t RAM::getTotalCapacity() const {
    return totalCapacityGB;
}

size_t RAM::getUsedMemory() const {
    return usedMemoryGB;
}

size_t RAM::getAvailableMemory() const {
    return totalCapacityGB - usedMemoryGB;
}

std::string RAM::getDiagnostics() const {
    return Component::getDiagnostics() +
           " | Memory: " + std::to_string(usedMemoryGB) + "/" +
           std::to_string(totalCapacityGB) + " GB (" +
           std::to_string((int)getUsagePercent()) + "%)";
}
