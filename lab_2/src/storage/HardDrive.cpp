#include "../../include/storage/HardDrive.h"
#include "../../include/exceptions/Exceptions.h"
#include <stdexcept>

HardDrive::HardDrive(const std::string& name, const std::string& manufacturer,
                     size_t capacity, bool ssd, double price)
    : Component(name, manufacturer, price),
      capacityGB(capacity), rpm(ssd ? 0 : 7200), isSSD(ssd),
      controller(ssd ? "NVMe" : "SATA", 1),
      readSpeedMBps(ssd ? 3500 : 150),
      writeSpeedMBps(ssd ? 3000 : 140) {

    partitions.emplace_back("C:", capacity / 2, "NTFS");
    partitions.emplace_back("D:", capacity / 2, "NTFS");
}

void HardDrive::write(const std::string& data) {
    if (!powered) throw std::runtime_error("Drive is off");
    partitions[0].writeFile("data.bin", data.length() / (1024*1024));
}

std::string HardDrive::read() {
    if (!powered) throw std::runtime_error("Drive is off");
    return "Data from drive";
}

void HardDrive::formatPartition(int partitionIndex) {
    if (partitionIndex >= 0 && partitionIndex < (int)partitions.size()) {
        partitions[partitionIndex].format();
    }
}

void HardDrive::defragment() {
    if (isSSD) return;
    // HDD defragmentation logic
}

void HardDrive::createPartition(const std::string& label, size_t sizeGB) {
    partitions.emplace_back(label, sizeGB, "NTFS");
}

size_t HardDrive::getCapacity() const {
    return capacityGB;
}

bool HardDrive::isSolidState() const {
    return isSSD;
}

double HardDrive::getReadSpeed() const {
    return readSpeedMBps;
}

double HardDrive::getWriteSpeed() const {
    return writeSpeedMBps;
}

int HardDrive::getPartitionCount() const {
    return partitions.size();
}

std::string HardDrive::getDiagnostics() const {
    return Component::getDiagnostics() +
           " | Type: " + (isSSD ? "SSD" : "HDD") +
           " | " + std::to_string(capacityGB) + " GB" +
           " | Speed: " + std::to_string((int)readSpeedMBps) + " MB/s";
}
