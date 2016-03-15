#ifndef MAINSERVER_MAINSERVER_HPP
#define MAINSERVER_MAINSERVER_HPP

#include <ctime>
#include <fstream>
#include <iostream>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Time.hpp>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>

#include "../PacketType.hpp"
#include "Client.hpp"
#include "GameServer.hpp"

namespace MainServer
{

class MainServer
{
    public:
        MainServer();
        ~MainServer();

        void start();
        void stop();
        bool isRunning() const;

        void listClients();
        void listServers();

        void handleCommand(std::string const& command);

    protected:
        void sendToAllClients(sf::Packet& packet);
        void sendToAllServers(sf::Packet& packet);

        void write(std::string const& message);

        int getClientId(sf::IpAddress address, sf::Uint16 port);
        int getServerId(sf::IpAddress address, sf::Uint16 port);

        void runClient();
        void runServer();

        void writeServerList(sf::Packet& packet);

    protected:
        bool mRunning;

        sf::Thread mClientThread;
        sf::Thread mServerThread;

        sf::UdpSocket mClientSocket;
        sf::UdpSocket mServerSocket;

        std::vector<Client> mClients;
        std::vector<GameServer> mServers;

        std::ofstream mLog;

        sf::Uint16 mClientPort;
        sf::Uint16 mServerPort;

        sf::Time mClientTimeout;
        sf::Time mServerTimeout;
};

} // namespace MainServer

#endif // MAINSERVER_MAINSERVER_HPP
