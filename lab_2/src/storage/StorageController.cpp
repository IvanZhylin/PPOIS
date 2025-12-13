#include "../../include/storage/StorageController.h"

StorageController::StorageController(const std::string& iface, int maxDev)
    : interface(iface), maxDevices(maxDev) {}

std::string StorageController::getInterface() const {
    return interface;
}

int StorageController::getMaxDevices() const {
    return maxDevices;
}

void StorageController::optimizeIO() {
    // Optimization logic
}
