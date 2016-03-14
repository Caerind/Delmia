#ifndef MAINSERVER_HPP
#define MAINSERVER_HPP

#include "../Server/Server.hpp"

class MainServer : public on::Server<MainPeer>
{
    public:
        MainServer();
    protected:
    private:
};

#endif // MAINSERVER_HPP
