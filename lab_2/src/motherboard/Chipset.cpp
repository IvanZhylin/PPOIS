#include "../../include/motherboard/Chipset.h"
#include <stdexcept>
Chipset::Chipset(const std::string& mdl, int usb, int sata, bool oc)
    : model(mdl), usbPorts(usb), sataPorts(sata), overclockSupport(oc) {}
void Chipset::enableOverclocking() {
    if (!overclockSupport) throw std::runtime_error("OC not supported");
}
int Chipset::getUSBPorts() const { return usbPorts; }
std::string Chipset::getModel() const { return model; }
