#include "../../include/graphics/GraphicsCard.h"
#include <stdexcept>

GraphicsCard::GraphicsCard(const std::string& name, const std::string& manufacturer,
                           int cores, double freq, size_t vramGB, double price)
    : Component(name, manufacturer, price),
      gpu(cores, freq, vramGB),
      displayPorts(3), hdmiPorts(1), powerConsumption(250.0) {}

void GraphicsCard::render3DScene() {
    if (!powered) throw std::runtime_error("GPU is off");
    gpu.renderFrame();
}

void GraphicsCard::computeShaders() {
    if (!powered) throw std::runtime_error("GPU is off");
    gpu.computeShaders();
}

double GraphicsCard::getTemperature() const {
    return gpu.getTemperature();
}

void GraphicsCard::cool(double amount) {
    gpu.cool(amount);
}

int GraphicsCard::getDisplayPorts() const {
    return displayPorts;
}

int GraphicsCard::getHDMIPorts() const {
    return hdmiPorts;
}

double GraphicsCard::getPowerConsumption() const {
    return powerConsumption;
}

std::string GraphicsCard::getDiagnostics() const {
    return Component::getDiagnostics() +
           " | GPU Temp: " + std::to_string((int)gpu.getTemperature()) + "C" +
           " | Cores: " + std::to_string(gpu.getCores());
}
