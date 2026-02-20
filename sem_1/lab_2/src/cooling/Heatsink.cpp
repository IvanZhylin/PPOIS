#include "../../include/cooling/Heatsink.h"
Heatsink::Heatsink(const std::string& mat, int pipes, double conductivity)
    : material(mat), heatpipes(pipes), thermalConductivity(conductivity) {}
double Heatsink::dissipateHeat(double temperature) {
    return temperature * thermalConductivity * 0.01;
}
std::string Heatsink::getMaterial() const { return material; }
int Heatsink::getHeatpipes() const { return heatpipes; }
