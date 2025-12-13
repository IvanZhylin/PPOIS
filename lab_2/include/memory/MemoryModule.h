#ifndef MEMORY_MODULE_H
#define MEMORY_MODULE_H

#include <string>
#include <cstdint>
#include <cstddef>
#include <map>

class MemoryModule {
private:
    size_t capacityGB;
    int frequencyMHz;
    std::string type;
    std::map<uint64_t, uint8_t> data;
    size_t usedBytes;

public:
    MemoryModule(size_t capacity, int freq, const std::string& t);

    void write(uint64_t address, uint8_t value);
    uint8_t read(uint64_t address) const;
    void clear();

    size_t getCapacity() const;
    int getFrequency() const;
    std::string getType() const;
    size_t getUsedBytes() const;
};

#endif // MEMORY_MODULE_H
