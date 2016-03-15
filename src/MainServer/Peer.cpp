#include "Peer.hpp"

namespace MainServer
{

Peer::Peer()
{
}

void Peer::setAddress(sf::IpAddress const& address)
{
    mAddress = address;
}

void Peer::setPort(sf::Uint16 const& port)
{
    mPort = port;
}

void Peer::setConnected(bool connected)
{
    mConnected = connected;
}

void Peer::resetLastPacketTime()
{
    mLastPacketTime.restart();
}

sf::IpAddress Peer::getAddress() const
{
    return mAddress;
}

sf::Uint16 Peer::getPort() const
{
    return mPort;
}

bool Peer::isConnected() const
{
    return mConnected;
}

sf::Time Peer::getLastPacketTime() const
{
    return mLastPacketTime.getElapsedTime();
}

} // namespace MainServer
