#include "../../include/graphics/GPU.h"
#include "../../include/exceptions/Exceptions.h"
#include <algorithm>

GPU::GPU(int cores, double freq, size_t vramGB)
    : cudaCores(cores), frequency(freq), temperature(40.0),
      vram(vramGB, 14000) {}

void GPU::renderFrame() {
    temperature += 3.0;
    if (temperature > 90.0) {
        throw GPUOverheatException();  
    }
}


void GPU::computeShaders() {
    renderFrame();
}

double GPU::getTemperature() const {
    return temperature;
}

void GPU::cool(double amount) {
    temperature = std::max(25.0, temperature - amount);
}

int GPU::getCores() const {
    return cudaCores;
}

double GPU::getFrequency() const {
    return frequency;
}

VRAM& GPU::getVRAM() {
    return vram;
}
