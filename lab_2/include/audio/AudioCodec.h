#ifndef AUDIO_CODEC_H
#define AUDIO_CODEC_H
#include <string>
class AudioCodec {
private:
    std::string codecName;
    int bitDepth;
    int sampleRate;
public:
    AudioCodec(const std::string& name, int bits, int rate);
    void encode();
    void decode();
    std::string getName() const;
};
#endif
