#ifndef SOUND_CARD_H
#define SOUND_CARD_H
#include "../interfaces/Interfaces.h"
#include "AudioCodec.h"
class SoundCard : public Component {
private:
    AudioCodec codec;
    int channels;
    double snrDB;
public:
    SoundCard(const std::string& name, const std::string& manufacturer,
              int ch, double price);
    void playSound();
    void recordSound();
    void adjustVolume(int level);
    int getChannels() const;
    std::string getDiagnostics() const override;
};
#endif
