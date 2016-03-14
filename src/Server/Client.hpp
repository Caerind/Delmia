#ifndef ON_CLIENT_HPP
#define ON_CLIENT_HPP

// Standards Libs
#include <functional>
#include <map>

// Own files
#include "Connection.hpp"

namespace on
{

class Client : public Connection
{
    public:
        Client();
        virtual ~Client();

        void handlePackets();

        sf::IpAddress getRemoteAddress();

        std::string getUsername() const;

    protected:
        std::string mUsername;

        sf::Thread mThread;

        std::map<sf::Int32,std::function<void(sf::Packet&)>> mPacketResponses;
};

}

#endif // ON_CLIENT_HPP
