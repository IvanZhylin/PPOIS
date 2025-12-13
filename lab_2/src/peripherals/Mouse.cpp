#include "../../include/peripherals/Mouse.h"
Mouse::Mouse(const std::string& name, int dpiValue, bool isWireless)
    : USBDevice(name, "USB 2.0", 12),
      dpi(dpiValue), buttonCount(5), wireless(isWireless) {}
void Mouse::connect() {}
void Mouse::disconnect() {}
void Mouse::click(int button) {}
void Mouse::setDPI(int newDPI) { dpi = newDPI; }
int Mouse::getDPI() const { return dpi; }
