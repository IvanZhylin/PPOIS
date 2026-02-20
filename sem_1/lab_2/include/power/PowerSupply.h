#ifndef POWER_SUPPLY_H
#define POWER_SUPPLY_H
#include "../interfaces/Interfaces.h"
#include "PowerRail.h"
#include <vector>
class PowerSupply : public Component {
private:
    int wattage;
    std::string efficiency;
    std::vector<PowerRail> rails;
    bool modular;
    double currentLoad;
public:
    PowerSupply(const std::string& name, const std::string& manufacturer,
                int watts, const std::string& eff, double price);
    void supplyPower(double watts);
    double getEfficiency() const;
    double getLoadPercent() const;
    int getWattage() const;
    std::string getDiagnostics() const override;
};
#endif
