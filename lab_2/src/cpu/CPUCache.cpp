#include "../../include/cpu/CPUCache.h"

CPUCache::CPUCache(int lvl, size_t size) 
    : level(lvl), sizeKB(size), hitCount(0), missCount(0) {}

bool CPUCache::read(uint64_t address, uint64_t& value) {
    auto it = cache.find(address);
    if (it != cache.end()) {
        value = it->second;
        hitCount++;
        return true;
    }
    missCount++;
    return false;
}

void CPUCache::write(uint64_t address, uint64_t value) {
    cache[address] = value;
}

void CPUCache::invalidate(uint64_t address) {
    cache.erase(address);
}

void CPUCache::flush() {
    cache.clear();
}

double CPUCache::getHitRate() const {
    int total = hitCount + missCount;
    return total > 0 ? (double)hitCount / total : 0.0;
}

int CPUCache::getLevel() const {
    return level;
}

size_t CPUCache::getSize() const {
    return sizeKB;
}

int CPUCache::getHitCount() const {
    return hitCount;
}

int CPUCache::getMissCount() const {
    return missCount;
}
