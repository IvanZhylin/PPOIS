#include "../../include/cpu/RegisterFile.h"

RegisterFile::RegisterFile(int count) : registerCount(count) {
    registers["EAX"] = 0;
    registers["EBX"] = 0;
    registers["ECX"] = 0;
    registers["EDX"] = 0;
}

void RegisterFile::setRegister(const std::string& name, uint64_t value) {
    registers[name] = value;
}

uint64_t RegisterFile::getRegister(const std::string& name) const {
    auto it = registers.find(name);
    return (it != registers.end()) ? it->second : 0;
}

void RegisterFile::clear() {
    for (auto& pair : registers) {
        pair.second = 0;
    }
}

int RegisterFile::getCount() const {
    return registerCount;
}
