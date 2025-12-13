#include "../../include/os/OperatingSystem.h"
OperatingSystem::OperatingSystem(const std::string& name, const std::string& ver)
    : osName(name), version(ver), filesystem("NTFS"), running(false) {}
void OperatingSystem::boot() {
    running = true;
    processes.emplace_back(0, "System", 100);
}
void OperatingSystem::shutdown() {
    for (auto& proc : processes) {
        proc.terminate();
    }
    running = false;
}
void OperatingSystem::startProcess(const std::string& name, size_t memory) {
    int pid = processes.size();
    processes.emplace_back(pid, name, memory);
}
void OperatingSystem::killProcess(int pid) {
    auto it = std::find_if(processes.begin(), processes.end(),
                          [pid](const Process& p) { return p.getPID() == pid; });
    if (it != processes.end()) {
        it->terminate();
        processes.erase(it);
    }
}
std::string OperatingSystem::getName() const { return osName; }
bool OperatingSystem::isRunning() const { return running; }
