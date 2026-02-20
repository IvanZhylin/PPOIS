#include "../../include/cpu/ControlUnit.h"

ControlUnit::ControlUnit() : programCounter(0), halted(false) {}

void ControlUnit::fetch(const std::string& instruction) {
    currentInstruction = instruction;
}

void ControlUnit::decode() {
    if (currentInstruction.empty()) {
        throw InvalidInstructionException("empty");
    }
}

void ControlUnit::execute(RegisterFile& regFile, ALU& alu) {
    if (currentInstruction == "ADD") {
        uint64_t a = regFile.getRegister("EAX");
        uint64_t b = regFile.getRegister("EBX");
        regFile.setRegister("EAX", alu.add(a, b));
    } else if (currentInstruction == "MUL") {
        uint64_t a = regFile.getRegister("EAX");
        uint64_t b = regFile.getRegister("EBX");
        regFile.setRegister("EAX", alu.multiply(a, b));
    }
    programCounter++;
}

void ControlUnit::incrementPC() {
    programCounter++;
}

uint64_t ControlUnit::getPC() const {
    return programCounter;
}

void ControlUnit::halt() {
    halted = true;
}

bool ControlUnit::isHalted() const {
    return halted;
}
