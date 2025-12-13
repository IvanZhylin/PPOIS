#include "../../include/cooling/Fan.h"
#include <algorithm>
Fan::Fan(int maxRPM, int size) 
    : rpmCurrent(0), rpmMax(maxRPM), sizeMM(size), noiseLevel(20.0) {}
void Fan::setSpeed(int rpm) {
    rpmCurrent = std::min(rpm, rpmMax);
    noiseLevel = 20.0 + (rpmCurrent / (double)rpmMax) * 30.0;
}
void Fan::stop() {
    rpmCurrent = 0;
    noiseLevel = 0;
}
double Fan::getCoolingPower() const {
    return (rpmCurrent / (double)rpmMax) * 50.0;
}
int Fan::getRPM() const { return rpmCurrent; }
double Fan::getNoiseLevel() const { return noiseLevel; }
