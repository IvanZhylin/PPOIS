#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <cstddef>

class Process {
private:
    int pid;
    std::string name;
    size_t memoryUsageMB;
    double cpuUsagePercent;
public:
    Process(int id, const std::string& n, size_t mem);
    void execute();
    void suspend();
    void terminate();
    int getPID() const;
    size_t getMemoryUsage() const;
};
#endif
