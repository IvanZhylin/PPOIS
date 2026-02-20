#include "../../include/memory/MemoryModule.h"
#include "../../include/exceptions/Exceptions.h"


MemoryModule::MemoryModule(size_t capacity, int freq, const std::string& t)
    : capacityGB(capacity), frequencyMHz(freq), type(t), usedBytes(0) {
    // Sparse storage - не выделяем реальную память!
}

void MemoryModule::write(uint64_t address, uint8_t value) {
    uint64_t maxAddress = static_cast<uint64_t>(capacityGB) * 1024ULL * 1024ULL * 1024ULL;
    if (address >= maxAddress) throw MemoryOverflowException();

    if (data.find(address) == data.end()) {
        usedBytes++;
    }
    data[address] = value;
}

uint8_t MemoryModule::read(uint64_t address) const {
    uint64_t maxAddress = static_cast<uint64_t>(capacityGB) * 1024ULL * 1024ULL * 1024ULL;
    if (address >= maxAddress) throw MemoryOverflowException();

    auto it = data.find(address);
    return (it != data.end()) ? it->second : 0;
}

void MemoryModule::clear() {
    data.clear();
    usedBytes = 0;
}

size_t MemoryModule::getCapacity() const { return capacityGB; }
int MemoryModule::getFrequency() const { return frequencyMHz; }
std::string MemoryModule::getType() const { return type; }
size_t MemoryModule::getUsedBytes() const { return usedBytes; }
