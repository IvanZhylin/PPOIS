#include "../../include/peripherals/Monitor.h"
#include <stdexcept>
Monitor::Monitor(const std::string& name, const std::string& manufacturer,
                 int width, int height, int refresh, double price)
    : Component(name, manufacturer, price),
      resolutionWidth(width), resolutionHeight(height),
      refreshRate(refresh), panelType("IPS"), diagonalInches(27.0) {}
void Monitor::displayImage() {
    if (!powered) throw std::runtime_error("Monitor is off");
}
void Monitor::changeResolution(int width, int height) {
    resolutionWidth = width;
    resolutionHeight = height;
}
void Monitor::changeRefreshRate(int rate) {
    refreshRate = rate;
}
int Monitor::getWidth() const { return resolutionWidth; }
int Monitor::getHeight() const { return resolutionHeight; }
std::string Monitor::getDiagnostics() const {
    return Component::getDiagnostics() +
           " | " + std::to_string(resolutionWidth) + "x" +
           std::to_string(resolutionHeight) + "@" +
           std::to_string(refreshRate) + "Hz";
}
