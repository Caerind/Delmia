#ifndef PACKETTYPE_HPP_INCLUDED
#define PACKETTYPE_HPP_INCLUDED

// Standards Libs
#include <string>

// SFML Network
#include <SFML/Network/Packet.hpp>

// Own files
#include "Server/Message.hpp"

namespace Packet
{

enum Type
{
    None = 0,
    // Server To Client
    ClientJoined,
    ClientLeft,
    ServerStopped,
    ServerMessage,
    Banned,
    Kicked,

    // Client To Server
    Login,
    Disconnect,
    ClientMessage,

};

// ClientJoined
void createClientJoinedPacket(sf::Packet& packet, std::string const& username);
void readClientJoinedPacket(sf::Packet& packet, std::string& username);

// ClientLeft
void createClientLeftPacket(sf::Packet& packet, std::string const& username);
void readClientLeftPacket(sf::Packet& packet, std::string& username);

// ServerStopped
void createServerStoppedPacket(sf::Packet& packet);

// ServerMessage
void createServerMessagePacket(sf::Packet& packet, Message const& message);
void readServerMessagePacket(sf::Packet& packet, Message& message);

// Banned
void createBannedPacket(sf::Packet& packet, Message const& message);
void readBannedPacket(sf::Packet& packet, Message& message);

// Kicked
void createKickedPacket(sf::Packet& packet, Message const& message);
void readKickedPacket(sf::Packet& packet, Message& message);

// Login
void createLoginPacket(sf::Packet& packet, std::string const& username, std::string const& password, sf::Uint32 const& port);
void readLoginPacket(sf::Packet& packet, std::string& username, std::string& password, sf::Uint32& port);

// Disconnect
void createDisconnectPacket(sf::Packet& packet);

// ClientMessage
void createClientMessagePacket(sf::Packet& packet, Message const& message);
void readClientMessagePacket(sf::Packet& packet, Message& message);

} // namespace Packet

#endif // PACKETTYPE_HPP_INCLUDED
