#ifndef MAINSERVER_CLIENT_HPP
#define MAINSERVER_CLIENT_HPP

#include "Peer.hpp"

namespace MainServer
{

class Client : public Peer
{
    public:
        Client();

        void setUsername(std::string const& username);

        std::string getUsername() const;

    protected:
        std::string mUsername;
};

} // namespace MainServer

#endif // MAINSERVER_CLIENT_HPP
