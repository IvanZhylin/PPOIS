#ifndef NETWORK_PACKET_H
#define NETWORK_PACKET_H

#include <string>
#include <cstddef>

class NetworkPacket {
private:
    std::string sourceIP;
    std::string destIP;
    std::string data;
    size_t size;
public:
    NetworkPacket(const std::string& src, const std::string& dst, const std::string& d);
    std::string getSource() const;
    std::string getDestination() const;
    size_t getSize() const;
};
#endif
