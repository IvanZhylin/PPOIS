#ifndef HARD_DRIVE_H
#define HARD_DRIVE_H

#include "../interfaces/Interfaces.h"
#include "Partition.h"
#include "StorageController.h"
#include <vector>
#include <string>
#include <cstddef>

class HardDrive : public Component {
private:
    size_t capacityGB;
    int rpm;
    bool isSSD;
    std::vector<Partition> partitions;
    StorageController controller;
    double readSpeedMBps;
    double writeSpeedMBps;

public:
    HardDrive(const std::string& name, const std::string& manufacturer,
              size_t capacity, bool ssd, double price);

    void write(const std::string& data);
    std::string read();
    void formatPartition(int partitionIndex);
    void defragment();
    void createPartition(const std::string& label, size_t sizeGB);

    size_t getCapacity() const;
    bool isSolidState() const;
    double getReadSpeed() const;
    double getWriteSpeed() const;
    int getPartitionCount() const;
    std::string getDiagnostics() const override;
};

#endif // HARD_DRIVE_H
