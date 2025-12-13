#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

#include <map>
#include <string>
#include <cstdint>

class RegisterFile {
private:
    std::map<std::string, uint64_t> registers;
    int registerCount;

public:
    RegisterFile(int count);

    void setRegister(const std::string& name, uint64_t value);
    uint64_t getRegister(const std::string& name) const;
    void clear();
    int getCount() const;
};

#endif // REGISTER_FILE_H
