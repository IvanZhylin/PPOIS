#ifndef STORAGE_CONTROLLER_H
#define STORAGE_CONTROLLER_H

#include <string>

class StorageController {
private:
    std::string interface;
    int maxDevices;

public:
    StorageController(const std::string& iface, int maxDev);

    std::string getInterface() const;
    int getMaxDevices() const;
    void optimizeIO();
};

#endif // STORAGE_CONTROLLER_H
