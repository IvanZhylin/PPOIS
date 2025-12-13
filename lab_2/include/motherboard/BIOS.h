#ifndef BIOS_H
#define BIOS_H
#include <string>
#include <map>
class BIOS {
private:
    std::string version;
    std::map<std::string, std::string> settings;
public:
    BIOS(const std::string& ver);
    void updateFirmware(const std::string& newVersion);
    void changeSetting(const std::string& key, const std::string& value);
    void reset();
    std::string getVersion() const;
};
#endif
