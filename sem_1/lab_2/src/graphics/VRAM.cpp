#include "../../include/graphics/VRAM.h"
#include "../../include/exceptions/Exceptions.h"

VRAM::VRAM(size_t capacity, int freq)
    : capacityGB(capacity), frequencyMHz(freq), usedMemory(0) {}

void VRAM::allocateTexture(size_t sizeMB) {
    usedMemory += sizeMB;
    if (usedMemory > capacityGB * 1024) throw MemoryOverflowException();
}

void VRAM::freeTexture(size_t sizeMB) {
    usedMemory = (sizeMB > usedMemory) ? 0 : usedMemory - sizeMB;
}

void VRAM::clear() {
    usedMemory = 0;
}

size_t VRAM::getCapacity() const {
    return capacityGB;
}

size_t VRAM::getUsed() const {
    return usedMemory;
}

size_t VRAM::getFree() const {
    return (capacityGB * 1024) - usedMemory;
}

double VRAM::getUsagePercent() const {
    return (double)usedMemory / (capacityGB * 1024) * 100.0;
}
