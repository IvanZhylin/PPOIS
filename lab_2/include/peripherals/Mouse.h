#ifndef MOUSE_H
#define MOUSE_H
#include "USBDevice.h"
class Mouse : public USBDevice {
private:
    int dpi;
    int buttonCount;
    bool wireless;
public:
    Mouse(const std::string& name, int dpiValue, bool isWireless);
    void connect() override;
    void disconnect() override;
    void click(int button);
    void setDPI(int newDPI);
    int getDPI() const;
};
#endif
