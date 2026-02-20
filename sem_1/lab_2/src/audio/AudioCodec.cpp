#include "../../include/audio/AudioCodec.h"
AudioCodec::AudioCodec(const std::string& name, int bits, int rate)
    : codecName(name), bitDepth(bits), sampleRate(rate) {}
void AudioCodec::encode() {}
void AudioCodec::decode() {}
std::string AudioCodec::getName() const { return codecName; }
