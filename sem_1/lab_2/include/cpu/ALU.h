#ifndef ALU_H
#define ALU_H

#include <cstdint>
#include <stdexcept>

class ALU {
private:
    int bitWidth;
    uint64_t lastResult;
    bool carryFlag;
    bool zeroFlag;

    void updateFlags();

public:
    ALU(int width);

    uint64_t add(uint64_t a, uint64_t b);
    uint64_t subtract(uint64_t a, uint64_t b);
    uint64_t multiply(uint64_t a, uint64_t b);
    uint64_t divide(uint64_t a, uint64_t b);
    uint64_t bitwiseAnd(uint64_t a, uint64_t b);
    uint64_t bitwiseOr(uint64_t a, uint64_t b);

    bool getZeroFlag() const;
    bool getCarryFlag() const;
};

#endif // ALU_H
