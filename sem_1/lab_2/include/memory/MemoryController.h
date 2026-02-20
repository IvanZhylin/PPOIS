#ifndef MEMORY_CONTROLLER_H
#define MEMORY_CONTROLLER_H

class MemoryController {
private:
    int channels;
    double transferRate;

public:
    MemoryController(int ch, double rate);

    double getTransferRate() const;
    int getChannels() const;
    void optimizeAccess();
    void setTransferRate(double rate);
};

#endif // MEMORY_CONTROLLER_H
