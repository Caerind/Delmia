#include "GamePeer.hpp"
#include "GameServer.hpp"
#include "../Server/Server.hpp"

GamePeer::GamePeer()
: Peer()
{
}

GamePeer::~GamePeer()
{
    disconnect();
}

bool GamePeer::connect()
{
    if (!isConnected())
    {
        sf::Packet packet;
        if (getSocketIn().receive(packet) == sf::Socket::Done)
        {
            sf::Int32 packetType;
            packet >> packetType;
            // TODO : Change
            if (packetType == Packet::Type::Login)
            {
                sf::IpAddress ip = getSocketIn().getRemoteAddress();
                std::string username, password;
                sf::Uint32 port;
                // TODO : Change
                Packet::readLoginPacket(packet,username,password,port);

                // Test login AND isn't ban AND isn't banip
                // Test login in a db
                // TODO : Change
                //if (true && !server.isBanned(username) && !server.isBannedIp(ip) && !server.isConnected(username))
                if (true)
                {
                    if (getSocketOut().connect(ip,port,getTimeout()) == sf::Socket::Status::Done)
                    {
                        mUsername = username;
                        return Connection::connect();
                    }
                }
            }
        }
    }
    return isConnected();
}

std::string GamePeer::getUsername() const
{
    return mUsername;
}
