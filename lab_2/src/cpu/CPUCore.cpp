#include "../../include/cpu/CPUCore.h"
#include "../../include/exceptions/Exceptions.h"
#include <algorithm>

CPUCore::CPUCore(int id, double freq) 
    : coreId(id), frequency(freq), temperature(35.0),
      registerFile(16), alu(64), active(false) {}

void CPUCore::executeInstruction(const std::string& instruction) {
    controlUnit.fetch(instruction);
    controlUnit.decode();
    controlUnit.execute(registerFile, alu);
    temperature += 2.0;
    if (temperature > 95.0) throw CPUOverheatException();
}

double CPUCore::getTemperature() const {
    return temperature;
}

void CPUCore::cool(double amount) {
    temperature = std::max(20.0, temperature - amount);
}

int CPUCore::getId() const {
    return coreId;
}

double CPUCore::getFrequency() const {
    return frequency;
}

void CPUCore::setActive(bool a) {
    active = a;
}

bool CPUCore::isActive() const {
    return active;
}

void CPUCore::reset() {
    registerFile.clear();
    temperature = 35.0;
    active = false;
}
