#ifndef USB_DEVICE_H
#define USB_DEVICE_H
#include <string>
class USBDevice {
protected:
    std::string deviceName;
    std::string usbVersion;
    int transferSpeedMbps;
public:
    USBDevice(const std::string& name, const std::string& version, int speed);
    virtual ~USBDevice() = default;
    virtual void connect() = 0;
    virtual void disconnect() = 0;
    std::string getName() const;
    int getSpeed() const;
};
#endif
