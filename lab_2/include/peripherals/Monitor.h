#ifndef MONITOR_H
#define MONITOR_H
#include "../interfaces/Interfaces.h"
class Monitor : public Component {
private:
    int resolutionWidth;
    int resolutionHeight;
    int refreshRate;
    std::string panelType;
    double diagonalInches;
public:
    Monitor(const std::string& name, const std::string& manufacturer,
            int width, int height, int refresh, double price);
    void displayImage();
    void changeResolution(int width, int height);
    void changeRefreshRate(int rate);
    int getWidth() const;
    int getHeight() const;
    std::string getDiagnostics() const override;
};
#endif
