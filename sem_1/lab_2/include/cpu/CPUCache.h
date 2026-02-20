#ifndef CPU_CACHE_H
#define CPU_CACHE_H

#include <map>
#include <cstdint>
#include <cstddef>

class CPUCache {
private:
    int level;
    size_t sizeKB;
    std::map<uint64_t, uint64_t> cache;
    int hitCount;
    int missCount;

public:
    CPUCache(int lvl, size_t size);

    bool read(uint64_t address, uint64_t& value);
    void write(uint64_t address, uint64_t value);
    void invalidate(uint64_t address);
    void flush();

    double getHitRate() const;
    int getLevel() const;
    size_t getSize() const;
    int getHitCount() const;
    int getMissCount() const;
};

#endif // CPU_CACHE_H
