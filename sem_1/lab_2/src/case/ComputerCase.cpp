#include "../../include/case/ComputerCase.h"
ComputerCase::ComputerCase(const std::string& name, const std::string& manufacturer,
                           const std::string& ff, double price)
    : Component(name, manufacturer, price),
      formFactor(ff), driveBays(4), temperedGlass(true) {
    fans.emplace_back(120, 1200, "Front");
    fans.emplace_back(120, 1200, "Rear");
}
void ComputerCase::installFan(const CaseFan& fan) {
    fans.push_back(fan);
}
void ComputerCase::adjustAirflow() {
    for (auto& fan : fans) {
        fan.setSpeed(1500);
    }
}
std::string ComputerCase::getFormFactor() const { return formFactor; }
std::string ComputerCase::getDiagnostics() const {
    return Component::getDiagnostics() +
           " | Form: " + formFactor +
           " | Fans: " + std::to_string(fans.size());
}
