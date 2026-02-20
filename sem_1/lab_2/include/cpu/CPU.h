#ifndef CPU_H
#define CPU_H

#include "../interfaces/Interfaces.h"
#include "CPUCore.h"
#include "CPUCache.h"
#include <vector>
#include <string>

class CPU : public Component, public ICoolable {
private:
    std::vector<CPUCore> cores;
    std::vector<CPUCache> caches;
    double baseFrequency;
    double maxFrequency;
    int threadCount;
    std::string architecture;
    double tdp;

public:
    CPU(const std::string& name, const std::string& manufacturer,
        int coreCount, double freq, double price);

    void processInstruction(const std::string& instruction);
    double getTemperature() const override;
    void cool(double amount) override;
    void overclock(double multiplier);
    void resetAllCores();

    int getCoreCount() const;
    int getThreadCount() const;
    double getFrequency() const;
    std::string getArchitecture() const;
    double getTDP() const;
    std::string getDiagnostics() const override;
};

#endif // CPU_H
