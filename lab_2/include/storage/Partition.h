#ifndef PARTITION_H
#define PARTITION_H

#include <string>
#include <cstddef>

class Partition {
private:
    std::string label;
    size_t sizeGB;
    size_t usedGB;
    std::string filesystem;

public:
    Partition(const std::string& lbl, size_t size, const std::string& fs);

    void writeFile(const std::string& filename, size_t fileSize);
    void deleteFile(size_t fileSize);
    void format();

    size_t getSize() const;
    size_t getUsed() const;
    size_t getFree() const;
    double getUsagePercent() const;
    std::string getLabel() const;
    std::string getFilesystem() const;
};

#endif // PARTITION_H
