#ifndef GPU_H
#define GPU_H

#include "../interfaces/Interfaces.h"
#include "VRAM.h"
#include <cstddef>

class GPU : public ICoolable {
private:
    int cudaCores;
    double frequency;
    double temperature;
    VRAM vram;

public:
    GPU(int cores, double freq, size_t vramGB);

    void renderFrame();
    void computeShaders();
    double getTemperature() const override;
    void cool(double amount) override;

    int getCores() const;
    double getFrequency() const;
    VRAM& getVRAM();
};

#endif // GPU_H
