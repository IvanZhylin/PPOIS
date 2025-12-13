#ifndef FAN_H
#define FAN_H
class Fan {
private:
    int rpmCurrent;
    int rpmMax;
    int sizeMM;
    double noiseLevel;
public:
    Fan(int maxRPM, int size);
    void setSpeed(int rpm);
    void stop();
    double getCoolingPower() const;
    int getRPM() const;
    double getNoiseLevel() const;
};
#endif
