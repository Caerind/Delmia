#ifndef GAMEPEER_HPP
#define GAMEPEER_HPP

// Standards Libs
#include <iostream>
#include <string>

// Own files
#include "../Server/Peer.hpp"
#include "../PacketType.hpp"

class GameServer;

class GamePeer : public Peer
{
    public:
        GamePeer();
        virtual ~GamePeer();

        virtual bool connect();

        std::string getUsername() const;

    protected:
        std::string mUsername;
};

#endif // GAMEPEER_HPP
