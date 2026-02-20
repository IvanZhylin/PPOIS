#include "../../include/network/NetworkPacket.h"
NetworkPacket::NetworkPacket(const std::string& src, const std::string& dst, const std::string& d)
    : sourceIP(src), destIP(dst), data(d), size(d.length()) {}
std::string NetworkPacket::getSource() const { return sourceIP; }
std::string NetworkPacket::getDestination() const { return destIP; }
size_t NetworkPacket::getSize() const { return size; }
