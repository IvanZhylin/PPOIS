#include "../../include/power/PowerRail.h"
PowerRail::PowerRail(double v, double a, double w)
    : voltage(v), currentAmps(a), maxWattage(w) {}
double PowerRail::getPower() const { return voltage * currentAmps; }
double PowerRail::getVoltage() const { return voltage; }
bool PowerRail::isOverloaded() const { return getPower() > maxWattage; }
