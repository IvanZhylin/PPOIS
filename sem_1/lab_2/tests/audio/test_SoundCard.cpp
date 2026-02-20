#include <gtest/gtest.h>
#include "../../include/audio/SoundCard.h"

class SoundCardTest : public ::testing::Test {
protected:
    SoundCard* soundCard;
    
    void SetUp() override {
        soundCard = new SoundCard("Sound Blaster Z", "Creative", 8, 99.99);
    }
    
    void TearDown() override {
        delete soundCard;
    }
};

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ КОНСТРУКТОРА И ГЕТТЕРОВ
// ═══════════════════════════════════════════════════════════════════

TEST_F(SoundCardTest, InitialState) {
    EXPECT_EQ(soundCard->getName(), "Sound Blaster Z");
    EXPECT_EQ(soundCard->getManufacturer(), "Creative");
    EXPECT_EQ(soundCard->getChannels(), 8);
    EXPECT_DOUBLE_EQ(soundCard->getPrice(), 99.99);
    EXPECT_FALSE(soundCard->isPowered());
}

TEST_F(SoundCardTest, StereoSound) {
    SoundCard* stereo = new SoundCard("Basic Audio", "Realtek", 2, 29.99);
    EXPECT_EQ(stereo->getChannels(), 2);
    delete stereo;
}

TEST_F(SoundCardTest, SurroundSound51) {
    SoundCard* surround = new SoundCard("AX210", "ASUS", 6, 149.99);
    EXPECT_EQ(surround->getChannels(), 6);
    delete surround;
}

TEST_F(SoundCardTest, SurroundSound71) {
    SoundCard* surround = new SoundCard("Xonar", "ASUS", 8, 199.99);
    EXPECT_EQ(surround->getChannels(), 8);
    delete surround;
}

TEST_F(SoundCardTest, ZeroChannels) {
    SoundCard* zero = new SoundCard("Broken", "Unknown", 0, 0.0);
    EXPECT_EQ(zero->getChannels(), 0);
    delete zero;
}

TEST_F(SoundCardTest, ManyChannels) {
    SoundCard* many = new SoundCard("Pro Audio", "RME", 32, 999.99);
    EXPECT_EQ(many->getChannels(), 32);
    delete many;
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ PLAY SOUND
// ═══════════════════════════════════════════════════════════════════

TEST_F(SoundCardTest, PlaySoundWhenOff) {
    EXPECT_THROW(soundCard->playSound(), std::runtime_error);
}

TEST_F(SoundCardTest, PlaySoundWhenOn) {
    soundCard->powerOn();
    EXPECT_NO_THROW(soundCard->playSound());
}

TEST_F(SoundCardTest, PlaySoundMultipleTimes) {
    soundCard->powerOn();
    soundCard->playSound();
    soundCard->playSound();
    soundCard->playSound();
    EXPECT_TRUE(soundCard->isPowered());
}

TEST_F(SoundCardTest, PlaySoundAfterPowerOff) {
    soundCard->powerOn();
    soundCard->playSound();  // OK
    soundCard->powerOff();
    EXPECT_THROW(soundCard->playSound(), std::runtime_error);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ RECORD SOUND
// ═══════════════════════════════════════════════════════════════════

TEST_F(SoundCardTest, RecordSoundWhenOff) {
    EXPECT_THROW(soundCard->recordSound(), std::runtime_error);
}

TEST_F(SoundCardTest, RecordSoundWhenOn) {
    soundCard->powerOn();
    EXPECT_NO_THROW(soundCard->recordSound());
}

TEST_F(SoundCardTest, RecordSoundMultipleTimes) {
    soundCard->powerOn();
    soundCard->recordSound();
    soundCard->recordSound();
    soundCard->recordSound();
    EXPECT_TRUE(soundCard->isPowered());
}

TEST_F(SoundCardTest, RecordSoundAfterPowerOff) {
    soundCard->powerOn();
    soundCard->recordSound();  // OK
    soundCard->powerOff();
    EXPECT_THROW(soundCard->recordSound(), std::runtime_error);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ ADJUST VOLUME
// ═══════════════════════════════════════════════════════════════════

TEST_F(SoundCardTest, AdjustVolumeZero) {
    soundCard->adjustVolume(0);
    EXPECT_EQ(soundCard->getChannels(), 8);
}

TEST_F(SoundCardTest, AdjustVolumeMiddle) {
    soundCard->adjustVolume(50);
    EXPECT_EQ(soundCard->getChannels(), 8);
}

TEST_F(SoundCardTest, AdjustVolumeMax) {
    soundCard->adjustVolume(100);
    EXPECT_EQ(soundCard->getChannels(), 8);
}

TEST_F(SoundCardTest, AdjustVolumeNegative) {
    soundCard->adjustVolume(-10);
    EXPECT_EQ(soundCard->getChannels(), 8);
}

TEST_F(SoundCardTest, AdjustVolumeOverMax) {
    soundCard->adjustVolume(150);
    EXPECT_EQ(soundCard->getChannels(), 8);
}

TEST_F(SoundCardTest, AdjustVolumeMultipleTimes) {
    soundCard->adjustVolume(20);
    soundCard->adjustVolume(50);
    soundCard->adjustVolume(80);
    soundCard->adjustVolume(100);
    EXPECT_EQ(soundCard->getChannels(), 8);
}

// ═══════════════════════════════════════════════════════════════════
// ТЕСТЫ GET DIAGNOSTICS
// ═══════════════════════════════════════════════════════════════════

TEST_F(SoundCardTest, GetDiagnostics) {
    std::string diag = soundCard->getDiagnostics();
    
    EXPECT_NE(diag.find("Sound Blaster Z"), std::string::npos);
    EXPECT_NE(diag.find("Channels:"), std::string::npos);
    EXPECT_NE(diag.find("8"), std::string::npos);
    EXPECT_NE(diag.find("SNR:"), std::string::npos);
    EXPECT_NE(diag.find("dB"), std::string::npos);
}

TEST_F(SoundCardTest, GetDiagnosticsAfterOperations) {
    soundCard->powerOn();
    soundCard->playSound();
    soundCard->adjustVolume(75);
    
    std::string diag = soundCard->getDiagnostics();
    EXPECT_NE(diag.find("Channels: 8"), std::string::npos);
}

// ═══════════════════════════════════════════════════════════════════
// ИНТЕГРАЦИОННЫЕ ТЕСТЫ
// ═══════════════════════════════════════════════════════════════════

TEST_F(SoundCardTest, PlayAndRecord) {
    soundCard->powerOn();
    soundCard->playSound();
    soundCard->recordSound();
    soundCard->playSound();
    EXPECT_TRUE(soundCard->isPowered());
}

TEST_F(SoundCardTest, CompleteWorkflow) {
    // Включение
    soundCard->powerOn();
    EXPECT_TRUE(soundCard->isPowered());
    
    // Воспроизведение
    soundCard->playSound();
    
    // Настройка громкости
    soundCard->adjustVolume(50);
    
    // Запись
    soundCard->recordSound();
    
    // Диагностика
    std::string diag = soundCard->getDiagnostics();
    EXPECT_FALSE(diag.empty());
    
    // Выключение
    soundCard->powerOff();
    EXPECT_FALSE(soundCard->isPowered());
}
