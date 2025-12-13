#include "../../include/storage/Partition.h"
#include "../../include/exceptions/Exceptions.h"

Partition::Partition(const std::string& lbl, size_t size, const std::string& fs)
    : label(lbl), sizeGB(size), usedGB(0), filesystem(fs) {}

void Partition::writeFile(const std::string& filename, size_t fileSize) {
    if (usedGB + fileSize > sizeGB) throw DiskFullException();
    usedGB += fileSize;
}

void Partition::deleteFile(size_t fileSize) {
    usedGB = (fileSize > usedGB) ? 0 : usedGB - fileSize;
}

void Partition::format() {
    usedGB = 0;
}

size_t Partition::getSize() const {
    return sizeGB;
}

size_t Partition::getUsed() const {
    return usedGB;
}

size_t Partition::getFree() const {
    return sizeGB - usedGB;
}

double Partition::getUsagePercent() const {
    return (double)usedGB / sizeGB * 100.0;
}

std::string Partition::getLabel() const {
    return label;
}

std::string Partition::getFilesystem() const {
    return filesystem;
}
