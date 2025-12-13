#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "USBDevice.h"
class Keyboard : public USBDevice {
private:
    int keyCount;
    bool mechanical;
    bool rgbBacklight;
public:
    Keyboard(const std::string& name, bool mech, bool rgb);
    void connect() override;
    void disconnect() override;
    void pressKey(char key);
    void setBacklight(bool enabled);
};
#endif
