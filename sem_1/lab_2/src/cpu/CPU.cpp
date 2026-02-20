#include "../../include/cpu/CPU.h"
#include "../../include/exceptions/Exceptions.h"
#include <stdexcept>

CPU::CPU(const std::string& name, const std::string& manufacturer,
         int coreCount, double freq, double price)
    : Component(name, manufacturer, price),
      baseFrequency(freq), maxFrequency(freq * 1.5),
      threadCount(coreCount * 2), architecture("x86-64"), tdp(65.0) {

    for (int i = 0; i < coreCount; i++) {
        cores.emplace_back(i, freq);
    }

    caches.emplace_back(1, 64);
    caches.emplace_back(2, 512);
    caches.emplace_back(3, 8192);
}

void CPU::processInstruction(const std::string& instruction) {
    if (!powered) throw std::runtime_error("CPU is off");
    cores[0].executeInstruction(instruction);
}

double CPU::getTemperature() const {
    double sum = 0;
    for (const auto& core : cores) {
        sum += core.getTemperature();
    }
    return cores.empty() ? 0 : sum / cores.size();
}

void CPU::cool(double amount) {
    for (auto& core : cores) {
        core.cool(amount);
    }
}

void CPU::overclock(double multiplier) {
    baseFrequency *= multiplier;
    if (baseFrequency > maxFrequency) {
        throw CPUOverheatException();
    }
}

void CPU::resetAllCores() {
    for (auto& core : cores) {
        core.reset();
    }
}

int CPU::getCoreCount() const {
    return cores.size();
}

int CPU::getThreadCount() const {
    return threadCount;
}

double CPU::getFrequency() const {
    return baseFrequency;
}

std::string CPU::getArchitecture() const {
    return architecture;
}

double CPU::getTDP() const {
    return tdp;
}

std::string CPU::getDiagnostics() const {
    return Component::getDiagnostics() + 
           " | Cores: " + std::to_string(cores.size()) +
           " | Freq: " + std::to_string(baseFrequency) + " GHz" +
           " | Temp: " + std::to_string(getTemperature()) + "C";
}
