#ifndef VRAM_H
#define VRAM_H

#include <cstddef>

class VRAM {
private:
    size_t capacityGB;
    int frequencyMHz;
    size_t usedMemory;

public:
    VRAM(size_t capacity, int freq);

    void allocateTexture(size_t sizeMB);
    void freeTexture(size_t sizeMB);
    void clear();

    size_t getCapacity() const;
    size_t getUsed() const;
    size_t getFree() const;
    double getUsagePercent() const;
};

#endif // VRAM_H
