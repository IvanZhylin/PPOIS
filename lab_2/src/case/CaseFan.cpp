#include "../../include/case/CaseFan.h"
CaseFan::CaseFan(int s, int r, const std::string& pos)
    : size(s), rpm(r), position(pos) {}
void CaseFan::spin() {}
void CaseFan::setSpeed(int newRPM) { rpm = newRPM; }
std::string CaseFan::getPosition() const { return position; }
