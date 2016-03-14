#ifndef MAINSERVER_HPP
#define MAINSERVER_HPP

#include "../Server/Server.hpp"
#include "../PacketType.hpp"
#include "MainPeer.hpp"

class MainServer : public on::Server<MainPeer>
{
    public:
        MainServer();

        void initPacketResponses();
        void initCommands();

        void load();
        void start();
        void stop();

        void update(sf::Time dt);

        void onConnection(MainPeer& peer);
        void onDisconnection(MainPeer& peer);
};

#endif // MAINSERVER_HPP
