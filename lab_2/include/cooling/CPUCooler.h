#ifndef CPU_COOLER_H
#define CPU_COOLER_H
#include "../interfaces/Interfaces.h"
#include "Fan.h"
#include "Heatsink.h"
#include <vector>
class CPUCooler : public Component {
private:
    std::vector<Fan> fans;
    Heatsink heatsink;
    std::string coolingType;
    int tdpRating;
public:
    CPUCooler(const std::string& name, const std::string& manufacturer,
              const std::string& type, int tdp, double price);
    void coolComponent(ICoolable& component);
    void adjustFanSpeed(int rpm);
    int getTDPRating() const;
    std::string getDiagnostics() const override;
};
#endif
