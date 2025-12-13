#ifndef COMPUTER_CASE_H
#define COMPUTER_CASE_H
#include "../interfaces/Interfaces.h"
#include "CaseFan.h"
#include <vector>
class ComputerCase : public Component {
private:
    std::string formFactor;
    std::vector<CaseFan> fans;
    int driveBays;
    bool temperedGlass;
public:
    ComputerCase(const std::string& name, const std::string& manufacturer,
                 const std::string& ff, double price);
    void installFan(const CaseFan& fan);
    void adjustAirflow();
    std::string getFormFactor() const;
    std::string getDiagnostics() const override;
};
#endif
