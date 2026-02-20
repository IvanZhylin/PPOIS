#include "../../include/power/PowerSupply.h"
#include "../../include/exceptions/Exceptions.h"
PowerSupply::PowerSupply(const std::string& name, const std::string& manufacturer,
                         int watts, const std::string& eff, double price)
    : Component(name, manufacturer, price),
      wattage(watts), efficiency(eff), modular(true), currentLoad(0) {
    rails.emplace_back(12.0, 40.0, 480);
    rails.emplace_back(5.0, 20.0, 100);
    rails.emplace_back(3.3, 20.0, 66);
}
void PowerSupply::supplyPower(double watts) {
    if (watts > wattage) throw PowerSupplyFailureException();
    currentLoad = watts;
}
double PowerSupply::getEfficiency() const {
    if (efficiency == "80+ Platinum") return 0.92;
    if (efficiency == "80+ Gold") return 0.90;
    return 0.85;
}
double PowerSupply::getLoadPercent() const {
    return (currentLoad / wattage) * 100.0;
}
int PowerSupply::getWattage() const { return wattage; }
std::string PowerSupply::getDiagnostics() const {
    return Component::getDiagnostics() +
           " | " + std::to_string(wattage) + "W" +
           " | Load: " + std::to_string((int)getLoadPercent()) + "%";
}
