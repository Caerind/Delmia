#ifndef MAINSERVER_MAINSERVER_HPP
#define MAINSERVER_MAINSERVER_HPP

#include <ctime>
#include <fstream>
#include <iostream>

#include "../PacketType.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "GameServer.hpp"

namespace MainServer
{

class MainServer
{
    public:
        MainServer();
        ~MainServer();

        bool start();
        void stop();
        bool isRunning() const;

        void handleCommand(std::string command, sf::IpAddress const& address = sf::IpAddress::LocalHost, sf::Uint16 port = 0);

    protected:
        void write(std::string const& message);

        typedef std::function<void(std::string args, sf::IpAddress const& address, sf::Uint16 port)> Command;

    protected:
        bool mRunning;

        Server<Client> mClients;
        Server<GameServer> mServers;

        std::map<std::string,Command> mCommands;

        std::ofstream mLog;
};

} // namespace MainServer

#endif // MAINSERVER_MAINSERVER_HPP
