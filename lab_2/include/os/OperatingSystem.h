#ifndef OPERATING_SYSTEM_H
#define OPERATING_SYSTEM_H

#include "Process.h"
#include "FileSystem.h"
#include <vector>
#include <algorithm>
#include <cstddef>

class OperatingSystem {
private:
    std::string osName;
    std::string version;
    std::vector<Process> processes;
    FileSystem filesystem;
    bool running;
public:
    OperatingSystem(const std::string& name, const std::string& ver);
    void boot();
    void shutdown();
    void startProcess(const std::string& name, size_t memory);
    void killProcess(int pid);
    std::string getName() const;
    bool isRunning() const;
};
#endif
