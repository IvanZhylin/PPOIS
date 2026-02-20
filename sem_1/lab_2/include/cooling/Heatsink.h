#ifndef HEATSINK_H
#define HEATSINK_H
#include <string>
class Heatsink {
private:
    std::string material;
    int heatpipes;
    double thermalConductivity;
public:
    Heatsink(const std::string& mat, int pipes, double conductivity);
    double dissipateHeat(double temperature);
    std::string getMaterial() const;
    int getHeatpipes() const;
};
#endif
