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
};

#endif // MAINSERVER_HPP
