#include "../../include/os/Process.h"
Process::Process(int id, const std::string& n, size_t mem)
    : pid(id), name(n), memoryUsageMB(mem), cpuUsagePercent(0) {}
void Process::execute() {
    cpuUsagePercent += 10.0;
}
void Process::suspend() {
    cpuUsagePercent = 0;
}
void Process::terminate() {
    cpuUsagePercent = 0;
    memoryUsageMB = 0;
}
int Process::getPID() const { return pid; }
size_t Process::getMemoryUsage() const { return memoryUsageMB; }
