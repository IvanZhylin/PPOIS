#include "../../include/motherboard/BIOS.h"
#include "../../include/exceptions/Exceptions.h"
BIOS::BIOS(const std::string& ver) : version(ver) {
    settings["BootOrder"] = "HDD";
    settings["VirtualizationTech"] = "Enabled";
}
void BIOS::updateFirmware(const std::string& newVersion) {
    if (newVersion < version) throw FirmwareException();
    version = newVersion;
}
void BIOS::changeSetting(const std::string& key, const std::string& value) {
    settings[key] = value;
}
void BIOS::reset() {
    settings.clear();
    settings["BootOrder"] = "HDD";
}
std::string BIOS::getVersion() const { return version; }
