#include "../../include/network/NetworkAdapter.h"
#include "../../include/exceptions/Exceptions.h"
NetworkAdapter::NetworkAdapter(const std::string& name, const std::string& manufacturer,
                               int speed, bool wifi, double price)
    : Component(name, manufacturer, price),
      macAddress("00:1A:2B:3C:4D:5E"),
      ipAddress("192.168.1.100"),
      speedMbps(speed), wireless(wifi) {}
void NetworkAdapter::sendPacket(const NetworkPacket& packet) {
    if (!powered) throw NetworkConnectionException();
    packetQueue.push_back(packet);
}
void NetworkAdapter::receivePacket() {
    if (!powered) throw NetworkConnectionException();
    if (!packetQueue.empty()) {
        packetQueue.erase(packetQueue.begin());
    }
}
void NetworkAdapter::setIPAddress(const std::string& ip) {
    ipAddress = ip;
}
int NetworkAdapter::getSpeed() const { return speedMbps; }
std::string NetworkAdapter::getIP() const { return ipAddress; }
std::string NetworkAdapter::getDiagnostics() const {
    return Component::getDiagnostics() +
           " | IP: " + ipAddress +
           " | " + std::to_string(speedMbps) + " Mbps";
}
