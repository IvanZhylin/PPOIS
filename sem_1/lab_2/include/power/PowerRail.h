#ifndef POWER_RAIL_H
#define POWER_RAIL_H
class PowerRail {
private:
    double voltage;
    double currentAmps;
    double maxWattage;
public:
    PowerRail(double v, double a, double w);
    double getPower() const;
    double getVoltage() const;
    bool isOverloaded() const;
};
#endif
