#include "Peer.hpp"

Peer::Peer()
: mRemove(false)
, mUsername("")
, mPassword("")
{
}

Peer::~Peer()
{
    disconnect();
}

bool Peer::connect()
{
    if (!isConnected())
    {
        sf::Packet packet;
        if (getSocketIn().receive(packet) == sf::Socket::Done)
        {
            //std::cout << "Incoming connection" << std::endl;
            sf::Int32 packetType;
            packet >> packetType;
            if (packetType == Packet::Type::Login)
            {
                //std::cout << "Login packet received" << std::endl;
                sf::IpAddress address = getSocketIn().getRemoteAddress();
                std::string username;
                std::string password;
                sf::Uint16 port;
                packet >> port >> username >> password;
                // TODO : MySQL Checking
                if (getSocketOut().connect(address,port,getTimeout()) == sf::Socket::Done)
                {
                    //std::cout << "Valid connection to " << address.toString() << ":" << port << std::endl;
                    packet.clear();
                    packet << Packet::Type::LoginSuccess;
                    if (getSocketOut().send(packet) == sf::Socket::Done)
                    {
                        //std::cout << "Packet login success sent" << std::endl;
                        mUsername = username;
                        mPassword = password;
                        return Connection::connect();
                    }
                }
            }
        }
    }
    return isConnected();
}

void Peer::remove()
{
    mRemove = true;
}

bool Peer::needRemove() const
{
    return mRemove;
}

std::string Peer::getUsername() const
{
    return mUsername;
}

std::string Peer::getPassword() const
{
    return mPassword;
}
