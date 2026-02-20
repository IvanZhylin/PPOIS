#ifndef CHIPSET_H
#define CHIPSET_H
#include <string>
class Chipset {
private:
    std::string model;
    int usbPorts;
    int sataPorts;
    bool overclockSupport;
public:
    Chipset(const std::string& mdl, int usb, int sata, bool oc);
    void enableOverclocking();
    int getUSBPorts() const;
    std::string getModel() const;
};
#endif
