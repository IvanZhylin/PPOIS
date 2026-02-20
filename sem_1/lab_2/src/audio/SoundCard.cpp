#include "../../include/audio/SoundCard.h"
#include <stdexcept>
SoundCard::SoundCard(const std::string& name, const std::string& manufacturer,
                     int ch, double price)
    : Component(name, manufacturer, price),
      codec("Realtek ALC1220", 24, 192000),
      channels(ch), snrDB(120.0) {}
void SoundCard::playSound() {
    if (!powered) throw std::runtime_error("Sound card is off");
    codec.decode();
}
void SoundCard::recordSound() {
    if (!powered) throw std::runtime_error("Sound card is off");
    codec.encode();
}
void SoundCard::adjustVolume(int level) {}
int SoundCard::getChannels() const { return channels; }
std::string SoundCard::getDiagnostics() const {
    return Component::getDiagnostics() +
           " | Channels: " + std::to_string(channels) +
           " | SNR: " + std::to_string(snrDB) + "dB";
}
