#ifndef CPU_CORE_H
#define CPU_CORE_H

#include "../interfaces/Interfaces.h"
#include "RegisterFile.h"
#include "ALU.h"
#include "ControlUnit.h"
#include <string>

class CPUCore : public ICoolable {
private:
    int coreId;
    double frequency;
    double temperature;
    RegisterFile registerFile;
    ALU alu;
    ControlUnit controlUnit;
    bool active;

public:
    CPUCore(int id, double freq);

    void executeInstruction(const std::string& instruction);
    double getTemperature() const override;
    void cool(double amount) override;

    int getId() const;
    double getFrequency() const;
    void setActive(bool a);
    bool isActive() const;
    void reset();
};

#endif // CPU_CORE_H
