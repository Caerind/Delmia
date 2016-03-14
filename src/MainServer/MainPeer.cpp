#include "MainPeer.hpp"
#include "MainServer.hpp"
#include "../Server/Server.hpp"
#include "../PacketType.hpp"

MainPeer::MainPeer() : on::Peer()
{
}

MainPeer::~MainPeer()
{
    disconnect();
}

bool MainPeer::connect()
{
    if (!isConnected())
    {
        sf::Packet packet;
        if (getSocketIn().receive(packet) == sf::Socket::Done)
        {
            sf::Int32 packetType;
            packet >> packetType;
            if (packetType == Packet::Type::MS_Login)
            {
                sf::IpAddress ip = getSocketIn().getRemoteAddress();
                std::string username, password;
                sf::Uint32 port;

                packet >> username >> password >> port;

                // TODO : DB Check
                // Test login AND isn't ban AND isn't banip
                // Test login in a db
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
