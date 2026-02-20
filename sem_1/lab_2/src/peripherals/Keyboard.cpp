#include "../../include/peripherals/Keyboard.h"
Keyboard::Keyboard(const std::string& name, bool mech, bool rgb)
    : USBDevice(name, "USB 2.0", 12),
      keyCount(104), mechanical(mech), rgbBacklight(rgb) {}
void Keyboard::connect() {}
void Keyboard::disconnect() {}
void Keyboard::pressKey(char key) {}
void Keyboard::setBacklight(bool enabled) {}
