#ifndef GRAPHICS_CARD_H
#define GRAPHICS_CARD_H

#include "../interfaces/Interfaces.h"
#include "GPU.h"
#include <cstddef>

class GraphicsCard : public Component, public ICoolable {
private:
    GPU gpu;
    int displayPorts;
    int hdmiPorts;
    double powerConsumption;

public:
    GraphicsCard(const std::string& name, const std::string& manufacturer,
                 int cores, double freq, size_t vramGB, double price);

    void render3DScene();
    void computeShaders();
    double getTemperature() const override;
    void cool(double amount) override;

    int getDisplayPorts() const;
    int getHDMIPorts() const;
    double getPowerConsumption() const;
    std::string getDiagnostics() const override;
};

#endif // GRAPHICS_CARD_H
