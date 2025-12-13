#include "../../include/peripherals/USBDevice.h"
USBDevice::USBDevice(const std::string& name, const std::string& version, int speed)
    : deviceName(name), usbVersion(version), transferSpeedMbps(speed) {}
std::string USBDevice::getName() const { return deviceName; }
int USBDevice::getSpeed() const { return transferSpeedMbps; }
