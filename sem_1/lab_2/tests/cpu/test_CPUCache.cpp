#include <gtest/gtest.h>
#include "../../include/cpu/CPUCache.h"

class CPUCacheTest : public ::testing::Test {
protected:
    CPUCache* cache;
    
    void SetUp() override {
        cache = new CPUCache(1, 32);  // L1 cache, 32KB
    }
    
    void TearDown() override {
        delete cache;
    }
};

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ КОНСТРУКТОРА И ГЕТТЕРОВ
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUCacheTest, InitialState) {
    EXPECT_EQ(cache->getLevel(), 1);
    EXPECT_EQ(cache->getSize(), 32);
    EXPECT_EQ(cache->getHitCount(), 0);
    EXPECT_EQ(cache->getMissCount(), 0);
    EXPECT_DOUBLE_EQ(cache->getHitRate(), 0.0);
}

TEST_F(CPUCacheTest, DifferentLevels) {
    CPUCache* l2 = new CPUCache(2, 256);
    CPUCache* l3 = new CPUCache(3, 8192);
    
    EXPECT_EQ(l2->getLevel(), 2);
    EXPECT_EQ(l2->getSize(), 256);
    EXPECT_EQ(l3->getLevel(), 3);
    EXPECT_EQ(l3->getSize(), 8192);
    
    delete l2;
    delete l3;
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ WRITE
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUCacheTest, WriteValue) {
    cache->write(0x1000, 42);
    
    uint64_t value;
    bool hit = cache->read(0x1000, value);
    
    EXPECT_TRUE(hit);
    EXPECT_EQ(value, 42);
}

TEST_F(CPUCacheTest, WriteMultipleValues) {
    cache->write(0x1000, 100);
    cache->write(0x2000, 200);
    cache->write(0x3000, 300);
    
    uint64_t v1, v2, v3;
    EXPECT_TRUE(cache->read(0x1000, v1));
    EXPECT_TRUE(cache->read(0x2000, v2));
    EXPECT_TRUE(cache->read(0x3000, v3));
    
    EXPECT_EQ(v1, 100);
    EXPECT_EQ(v2, 200);
    EXPECT_EQ(v3, 300);
}

TEST_F(CPUCacheTest, OverwriteValue) {
    cache->write(0x1000, 42);
    cache->write(0x1000, 84);  // Перезапись
    
    uint64_t value;
    cache->read(0x1000, value);
    EXPECT_EQ(value, 84);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ READ - CACHE HIT
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUCacheTest, ReadHit) {
    cache->write(0x1000, 42);
    
    uint64_t value;
    bool hit = cache->read(0x1000, value);
    
    EXPECT_TRUE(hit);
    EXPECT_EQ(value, 42);
    EXPECT_EQ(cache->getHitCount(), 1);
    EXPECT_EQ(cache->getMissCount(), 0);
}

TEST_F(CPUCacheTest, MultipleHits) {
    cache->write(0x1000, 100);
    
    uint64_t value;
    bool hit1 = cache->read(0x1000, value);  // Hit 1
    bool hit2 = cache->read(0x1000, value);  // Hit 2
    bool hit3 = cache->read(0x1000, value);  // Hit 3
    
    EXPECT_TRUE(hit1);
    EXPECT_TRUE(hit2);
    EXPECT_TRUE(hit3);
    EXPECT_EQ(cache->getHitCount(), 3);
    EXPECT_EQ(cache->getMissCount(), 0);
    EXPECT_DOUBLE_EQ(cache->getHitRate(), 1.0);  // 100% = 1.0, не 100.0
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ READ - CACHE MISS
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUCacheTest, ReadMiss) {
    uint64_t value;
    bool hit = cache->read(0x9999, value);  // Адрес не в кеше
    
    EXPECT_FALSE(hit);
    EXPECT_EQ(cache->getHitCount(), 0);
    EXPECT_EQ(cache->getMissCount(), 1);
}

TEST_F(CPUCacheTest, MultipleMisses) {
    uint64_t value;
    cache->read(0x1111, value);  // Miss 1
    cache->read(0x2222, value);  // Miss 2
    cache->read(0x3333, value);  // Miss 3
    
    EXPECT_EQ(cache->getHitCount(), 0);
    EXPECT_EQ(cache->getMissCount(), 3);
    EXPECT_DOUBLE_EQ(cache->getHitRate(), 0.0);
}

TEST_F(CPUCacheTest, MixedHitsAndMisses) {
    cache->write(0x1000, 42);
    
    uint64_t value;
    cache->read(0x1000, value);  // Hit
    cache->read(0x2000, value);  // Miss
    cache->read(0x1000, value);  // Hit
    cache->read(0x3000, value);  // Miss
    
    EXPECT_EQ(cache->getHitCount(), 2);
    EXPECT_EQ(cache->getMissCount(), 2);
    EXPECT_DOUBLE_EQ(cache->getHitRate(), 0.5);  // 50%
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ HIT RATE
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUCacheTest, HitRateZeroWhenNoOperations) {
    EXPECT_DOUBLE_EQ(cache->getHitRate(), 0.0);
}

TEST_F(CPUCacheTest, HitRate100Percent) {
    cache->write(0x1000, 1);
    cache->write(0x2000, 2);
    
    uint64_t value;
    cache->read(0x1000, value);
    cache->read(0x2000, value);
    cache->read(0x1000, value);
    cache->read(0x2000, value);
    
    EXPECT_EQ(cache->getHitCount(), 4);
    EXPECT_EQ(cache->getMissCount(), 0);
    EXPECT_DOUBLE_EQ(cache->getHitRate(), 1.0);  // 100%
}

TEST_F(CPUCacheTest, HitRate75Percent) {
    cache->write(0x1000, 100);
    
    uint64_t value;
    cache->read(0x1000, value);  // Hit
    cache->read(0x1000, value);  // Hit
    cache->read(0x1000, value);  // Hit
    cache->read(0x2000, value);  // Miss
    
    // 3 hits, 1 miss = 75%
    EXPECT_EQ(cache->getHitCount(), 3);
    EXPECT_EQ(cache->getMissCount(), 1);
    EXPECT_DOUBLE_EQ(cache->getHitRate(), 0.75);
}

TEST_F(CPUCacheTest, HitRate25Percent) {
    cache->write(0x1000, 100);
    
    uint64_t value;
    cache->read(0x1000, value);  // Hit
    cache->read(0x2000, value);  // Miss
    cache->read(0x3000, value);  // Miss
    cache->read(0x4000, value);  // Miss
    
    // 1 hit, 3 misses = 25%
    EXPECT_EQ(cache->getHitCount(), 1);
    EXPECT_EQ(cache->getMissCount(), 3);
    EXPECT_DOUBLE_EQ(cache->getHitRate(), 0.25);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ INVALIDATE
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUCacheTest, InvalidateSingleEntry) {
    cache->write(0x1000, 42);
    cache->invalidate(0x1000);
    
    uint64_t value;
    bool hit = cache->read(0x1000, value);
    
    EXPECT_FALSE(hit);  // После invalidate - miss
    EXPECT_EQ(cache->getMissCount(), 1);
}

TEST_F(CPUCacheTest, InvalidateNonExistentEntry) {
    cache->write(0x1000, 42);
    cache->invalidate(0x9999);  // Инвалидация несуществующего адреса
    
    uint64_t value;
    bool hit = cache->read(0x1000, value);
    
    EXPECT_TRUE(hit);  // 0x1000 все еще в кеше
    EXPECT_EQ(value, 42);
}

TEST_F(CPUCacheTest, InvalidateOneOfMany) {
    cache->write(0x1000, 100);
    cache->write(0x2000, 200);
    cache->write(0x3000, 300);
    
    cache->invalidate(0x2000);  // Удаляем только 0x2000
    
    uint64_t value;
    EXPECT_TRUE(cache->read(0x1000, value));   // Hit
    EXPECT_FALSE(cache->read(0x2000, value));  // Miss (invalidated)
    EXPECT_TRUE(cache->read(0x3000, value));   // Hit
    
    EXPECT_EQ(cache->getHitCount(), 2);
    EXPECT_EQ(cache->getMissCount(), 1);
}

TEST_F(CPUCacheTest, InvalidateAndRewrite) {
    cache->write(0x1000, 42);
    cache->invalidate(0x1000);
    cache->write(0x1000, 84);  // Перезапись после инвалидации
    
    uint64_t value;
    bool hit = cache->read(0x1000, value);
    
    EXPECT_TRUE(hit);
    EXPECT_EQ(value, 84);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ FLUSH
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUCacheTest, FlushEmptyCache) {
    cache->flush();  // Flush пустого кеша
    EXPECT_DOUBLE_EQ(cache->getHitRate(), 0.0);
}

TEST_F(CPUCacheTest, FlushClearsAllEntries) {
    cache->write(0x1000, 100);
    cache->write(0x2000, 200);
    cache->write(0x3000, 300);
    
    cache->flush();
    
    uint64_t value;
    EXPECT_FALSE(cache->read(0x1000, value));
    EXPECT_FALSE(cache->read(0x2000, value));
    EXPECT_FALSE(cache->read(0x3000, value));
    
    EXPECT_EQ(cache->getMissCount(), 3);
}

TEST_F(CPUCacheTest, FlushDoesNotResetCounters) {
    cache->write(0x1000, 100);
    
    uint64_t value;
    cache->read(0x1000, value);  // Hit
    cache->read(0x2000, value);  // Miss
    
    cache->flush();
    
    // Счетчики НЕ сбрасываются после flush
    EXPECT_EQ(cache->getHitCount(), 1);
    EXPECT_EQ(cache->getMissCount(), 1);
}

TEST_F(CPUCacheTest, WriteAfterFlush) {
    cache->write(0x1000, 42);
    cache->flush();
    cache->write(0x2000, 84);  // Запись после flush
    
    uint64_t value;
    EXPECT_FALSE(cache->read(0x1000, value));  // Miss (flushed)
    EXPECT_TRUE(cache->read(0x2000, value));   // Hit (новая запись)
    EXPECT_EQ(value, 84);
}

// ═══════════════════════════════════════════════════════════════════
// ГРАНИЧНЫЕ СЛУЧАИ
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUCacheTest, ZeroAddress) {
    cache->write(0x0, 999);
    
    uint64_t value;
    bool hit = cache->read(0x0, value);
    
    EXPECT_TRUE(hit);
    EXPECT_EQ(value, 999);
}

TEST_F(CPUCacheTest, MaxAddress) {
    uint64_t maxAddr = UINT64_MAX;
    cache->write(maxAddr, 12345);
    
    uint64_t value;
    bool hit = cache->read(maxAddr, value);
    
    EXPECT_TRUE(hit);
    EXPECT_EQ(value, 12345);
}

TEST_F(CPUCacheTest, ZeroValue) {
    cache->write(0x1000, 0);
    
    uint64_t value = 999;  // Не ноль изначально
    bool hit = cache->read(0x1000, value);
    
    EXPECT_TRUE(hit);
    EXPECT_EQ(value, 0);
}

TEST_F(CPUCacheTest, MaxValue) {
    uint64_t maxVal = UINT64_MAX;
    cache->write(0x1000, maxVal);
    
    uint64_t value;
    bool hit = cache->read(0x1000, value);
    
    EXPECT_TRUE(hit);
    EXPECT_EQ(value, maxVal);
}

// ═══════════════════════════════════════════════════════════════════
// СТРЕСС-ТЕСТЫ
// ═══════════════════════════════════════════════════════════════════

TEST_F(CPUCacheTest, ManyEntries) {
    // Записываем 1000 записей
    for (uint64_t i = 0; i < 1000; i++) {
        cache->write(i * 0x1000, i);
    }
    
    // Читаем все обратно
    uint64_t value;
    for (uint64_t i = 0; i < 1000; i++) {
        EXPECT_TRUE(cache->read(i * 0x1000, value));
        EXPECT_EQ(value, i);
    }
    
    EXPECT_EQ(cache->getHitCount(), 1000);
    EXPECT_EQ(cache->getMissCount(), 0);
}

TEST_F(CPUCacheTest, RealisticWorkload) {
    // Симуляция реальной работы кеша
    cache->write(0x1000, 10);
    cache->write(0x2000, 20);
    cache->write(0x3000, 30);
    
    uint64_t value;
    
    // Несколько обращений к горячим данным
    cache->read(0x1000, value);  // Hit
    cache->read(0x1000, value);  // Hit
    cache->read(0x2000, value);  // Hit
    
    // Промах
    cache->read(0x9999, value);  // Miss
    
    // Еще хиты
    cache->read(0x3000, value);  // Hit
    cache->read(0x1000, value);  // Hit
    
    // Инвалидация
    cache->invalidate(0x2000);
    cache->read(0x2000, value);  // Miss
    
    EXPECT_EQ(cache->getHitCount(), 5);
    EXPECT_EQ(cache->getMissCount(), 2);
    EXPECT_DOUBLE_EQ(cache->getHitRate(), 5.0 / 7.0);
}
