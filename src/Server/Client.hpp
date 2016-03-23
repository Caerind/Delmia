#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <functional>
#include <map>

#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include "Connection.hpp"
#include "PacketType.hpp"
#include "Output.hpp"

class Client : public on::Connection, public on::Output
{
    public:
        Client();
        ~Client();

        typedef std::function<void(sf::Packet&)> Response;

        bool connect(sf::IpAddress const& address, sf::Uint16 port, std::string const& username, std::string const& password);
        void disconnect();

        void handleCommand(std::string const& command);

        void initPacketResponses();

    protected:
        void run();

    protected:
        sf::Thread mThread;
        sf::Mutex mMutex;
        bool mRunning;

        std::string mUsername;
        std::string mPassword;

        std::map<sf::Int32,Response> mPacketResponses;
};

#endif // CLIENT_HPP
