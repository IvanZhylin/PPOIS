#include "../../include/cooling/CPUCooler.h"
CPUCooler::CPUCooler(const std::string& name, const std::string& manufacturer,
                     const std::string& type, int tdp, double price)
    : Component(name, manufacturer, price),
      heatsink("Copper", 6, 385.0),
      coolingType(type), tdpRating(tdp) {
    fans.emplace_back(2000, 120);
    if (type == "AIO") {
        fans.emplace_back(2000, 120);
    }
}
void CPUCooler::coolComponent(ICoolable& component) {
    double temp = component.getTemperature();
    double coolingPower = 0;
    for (auto& fan : fans) {
        fan.setSpeed(1500);
        coolingPower += fan.getCoolingPower();
    }
    coolingPower += heatsink.dissipateHeat(temp);
    component.cool(coolingPower * 0.1);
}
void CPUCooler::adjustFanSpeed(int rpm) {
    for (auto& fan : fans) {
        fan.setSpeed(rpm);
    }
}
int CPUCooler::getTDPRating() const { return tdpRating; }
std::string CPUCooler::getDiagnostics() const {
    return Component::getDiagnostics() +
           " | Type: " + coolingType +
           " | TDP: " + std::to_string(tdpRating) + "W";
}
