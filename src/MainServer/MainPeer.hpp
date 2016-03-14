#ifndef MAINPEER_HPP
#define MAINPEER_HPP

#include <iostream>
#include <string>

#include "../Server/Peer.hpp"

class MainServer;

class MainPeer : public on::Peer
{
    public:
        MainPeer();
        virtual ~MainPeer();

        virtual bool connect();
};

#endif // MAINPEER_HPP
