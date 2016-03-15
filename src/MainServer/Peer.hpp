#ifndef MAINSERVER_PEER_HPP
#define MAINSERVER_PEER_HPP

#include <SFML/System/Clock.hpp>
#include <SFML/Network/IpAddress.hpp>

namespace MainServer
{

class Peer
{
    public:
        Peer();

        void setAddress(sf::IpAddress const& address);
        void setPort(sf::Uint16 const& port);
        void setConnected(bool connected);
        void resetLastPacketTime();

        sf::IpAddress getAddress() const;
        sf::Uint16 getPort() const;
        bool isConnected() const;
        sf::Time getLastPacketTime() const;

    protected:
        sf::IpAddress mAddress;
        sf::Uint16 mPort;
        bool mConnected;
        sf::Clock mLastPacketTime;
};

} // namespace MainServer

#endif // MAINSERVER_PEER_HPP
