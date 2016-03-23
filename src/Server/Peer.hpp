#ifndef PEER_HPP
#define PEER_HPP

#include "Connection.hpp"
#include "PacketType.hpp"

class Peer : public on::Connection
{
    public:
        Peer();
        ~Peer();

        bool connect();

        void remove();
        bool needRemove() const;

        std::string getUsername() const;
        std::string getPassword() const;

    protected:
        bool mRemove;
        std::string mUsername;
        std::string mPassword;
};

#endif // PEER_HPP
