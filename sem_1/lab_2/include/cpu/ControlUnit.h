#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include "RegisterFile.h"
#include "ALU.h"
#include "../exceptions/Exceptions.h"
#include <string>
#include <cstdint>

class ControlUnit {
private:
    uint64_t programCounter;
    std::string currentInstruction;
    bool halted;

public:
    ControlUnit();

    void fetch(const std::string& instruction);
    void decode();
    void execute(RegisterFile& regFile, ALU& alu);
    void incrementPC();
    void halt();

    uint64_t getPC() const;
    bool isHalted() const;
};

#endif // CONTROL_UNIT_H
