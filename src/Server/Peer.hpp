#ifndef ON_PEER_HPP
#define ON_PEER_HPP

// Own files
#include "Connection.hpp"

namespace on
{

class Peer : public Connection
{
    public:
        Peer();
        virtual ~Peer();

        virtual bool connect();

        void remove();
        bool needRemove() const;

        sf::IpAddress getRemoteAddress();

        std::string getUsername() const;

    protected:
        bool mRemove;
        std::string mUsername;
};

} // namespace on

#endif // ON_PEER_HPP
