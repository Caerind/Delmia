#ifndef ON_CLIENT_HPP
#define ON_CLIENT_HPP

#include <functional>
#include <map>
#include <queue>

#include "Connection.hpp"

namespace on
{

class Client : public Connection
{
    public:
        Client();
        virtual ~Client();

        void handlePackets();
        bool pollPacket(sf::Int32& packetType, sf::Packet& packet);

        sf::IpAddress getRemoteAddress();

        std::string getUsername() const;

    protected:
        std::string mUsername;

        sf::Thread mThread;

        std::map<sf::Int32,std::function<void(sf::Packet&)>> mPacketResponses;

        std::queue<std::pair<sf::Int32,sf::Packet>> mUnhandledPackets;
};

} // namespace on

#endif // ON_CLIENT_HPP
