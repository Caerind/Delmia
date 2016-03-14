#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <SFML/Network/TcpListener.hpp>
#include "../Server/Client.hpp"
#include "../Server/Message.hpp"
#include "../PacketType.hpp"

class Client : public on::Client
{
    public:
        static Client& instance();

        bool connect(sf::IpAddress const& ip, sf::Uint32 port, std::string const& username, std::string const& password);
        void disconnect(bool sendPacket);

        void initPacketResponses(); // Some packets are automagically handled

    private:
        static Client* mInstance;

        Client();
        virtual ~Client();

        std::string mPassword; // Hashed password, useful for auto reconnection
};

#endif // CLIENT_HPP
