#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

class HardwareException : public std::exception {
protected:
    std::string message;
public:
    explicit HardwareException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class CPUOverheatException : public HardwareException {
public:
    CPUOverheatException() : HardwareException("CPU temperature critical!") {}
};

class MemoryOverflowException : public HardwareException {
public:
    MemoryOverflowException() : HardwareException("Memory overflow detected!") {}
};

class DiskFullException : public HardwareException {
public:
    DiskFullException() : HardwareException("Disk space exhausted!") {}
};

class PowerSupplyFailureException : public HardwareException {
public:
    PowerSupplyFailureException() : HardwareException("Power supply failure!") {}
};

class NetworkConnectionException : public HardwareException {
public:
    NetworkConnectionException() : HardwareException("Network connection lost!") {}
};

class InvalidInstructionException : public std::exception {
private:
    std::string msg;
public:
    explicit InvalidInstructionException(const std::string& instruction) 
        : msg("Invalid instruction: " + instruction) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

class BusContentionException : public HardwareException {
public:
    BusContentionException() : HardwareException("Bus contention detected!") {}
};

class CacheCoherencyException : public HardwareException {
public:
    CacheCoherencyException() : HardwareException("Cache coherency violated!") {}
};

class InterruptException : public std::exception {
private:
    int interruptNumber;
public:
    explicit InterruptException(int num) : interruptNumber(num) {}
    const char* what() const noexcept override { return "Interrupt triggered!"; }
    int getInterruptNumber() const { return interruptNumber; }
};

class PortAccessException : public HardwareException {
public:
    PortAccessException() : HardwareException("Invalid port access!") {}
};

class BIOSException : public HardwareException {
public:
    BIOSException() : HardwareException("BIOS initialization failed!") {}
};

class FirmwareException : public HardwareException {
public:
    FirmwareException() : HardwareException("Firmware error!") {}
};

class GPUOverheatException : public HardwareException {
public:
    GPUOverheatException() : HardwareException("GPU temperature critical!") {}
};


#endif // EXCEPTIONS_H
