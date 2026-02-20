#include "../../include/memory/MemoryController.h"

MemoryController::MemoryController(int ch, double rate) 
    : channels(ch), transferRate(rate) {}

double MemoryController::getTransferRate() const {
    return transferRate;
}

int MemoryController::getChannels() const {
    return channels;
}

void MemoryController::optimizeAccess() {
    transferRate *= 1.1;
}

void MemoryController::setTransferRate(double rate) {
    transferRate = rate;
}
