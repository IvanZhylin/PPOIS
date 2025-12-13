#include "../../include/cpu/ALU.h"

ALU::ALU(int width) : bitWidth(width), lastResult(0), 
                      carryFlag(false), zeroFlag(false) {}

uint64_t ALU::add(uint64_t a, uint64_t b) {
    lastResult = a + b;
    updateFlags();
    return lastResult;
}

uint64_t ALU::subtract(uint64_t a, uint64_t b) {
    lastResult = a - b;
    updateFlags();
    return lastResult;
}

uint64_t ALU::multiply(uint64_t a, uint64_t b) {
    lastResult = a * b;
    updateFlags();
    return lastResult;
}

uint64_t ALU::divide(uint64_t a, uint64_t b) {
    if (b == 0) throw std::runtime_error("Division by zero");
    lastResult = a / b;
    updateFlags();
    return lastResult;
}

uint64_t ALU::bitwiseAnd(uint64_t a, uint64_t b) {
    lastResult = a & b;
    updateFlags();
    return lastResult;
}

uint64_t ALU::bitwiseOr(uint64_t a, uint64_t b) {
    lastResult = a | b;
    updateFlags();
    return lastResult;
}

bool ALU::getZeroFlag() const {
    return zeroFlag;
}

bool ALU::getCarryFlag() const {
    return carryFlag;
}

void ALU::updateFlags() {
    zeroFlag = (lastResult == 0);
    carryFlag = (lastResult > ((1ULL << bitWidth) - 1));
}
