#ifndef CASE_FAN_H
#define CASE_FAN_H
#include <string>
class CaseFan {
private:
    int size;
    int rpm;
    std::string position;
public:
    CaseFan(int s, int r, const std::string& pos);
    void spin();
    void setSpeed(int newRPM);
    std::string getPosition() const;
};
#endif
