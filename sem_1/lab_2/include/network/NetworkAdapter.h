#ifndef NETWORK_ADAPTER_H
#define NETWORK_ADAPTER_H
#include "../interfaces/Interfaces.h"
#include "NetworkPacket.h"
#include <vector>
class NetworkAdapter : public Component {
private:
    std::string macAddress;
    std::string ipAddress;
    int speedMbps;
    bool wireless;
    std::vector<NetworkPacket> packetQueue;
public:
    NetworkAdapter(const std::string& name, const std::string& manufacturer,
                   int speed, bool wifi, double price);
    void sendPacket(const NetworkPacket& packet);
    void receivePacket();
    void setIPAddress(const std::string& ip);
    int getSpeed() const;
    std::string getIP() const;
    std::string getDiagnostics() const override;
};
#endif
