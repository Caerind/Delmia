#include "PacketType.hpp"

namespace Packet
{

void createClientJoinedPacket(sf::Packet& packet, std::string const& username)
{
    packet << Packet::Type::ClientJoined << username;
}

void readClientJoinedPacket(sf::Packet& packet, std::string& username)
{
    packet >> username;
}

void createClientLeftPacket(sf::Packet& packet, std::string const& username)
{
    packet << Packet::Type::ClientLeft << username;
}

void readClientLeftPacket(sf::Packet& packet, std::string& username)
{
    packet >> username;
}

void createServerStoppedPacket(sf::Packet& packet)
{
    packet << Packet::Type::ServerStopped;
}

void createServerMessagePacket(sf::Packet& packet, Message const& message)
{
    packet << Packet::Type::ServerMessage << message;
}

void readServerMessagePacket(sf::Packet& packet, Message& message)
{
    packet >> message;
}

void createBannedPacket(sf::Packet& packet, Message const& message)
{
    packet << Packet::Type::Banned << message;
}

void readBannedPacket(sf::Packet& packet, Message& message)
{
    packet >> message;
}

void createKickedPacket(sf::Packet& packet, Message const& message)
{
    packet << Packet::Type::Banned << message;
}

void readKickedPacket(sf::Packet& packet, Message& message)
{
    packet >> message;
}

void readKickedPacket(sf::Packet& packet, Message& message);

void createLoginPacket(sf::Packet& packet, std::string const& username, std::string const& password, sf::Uint32 const& port)
{
    packet << Packet::Type::Login << username << password << port;
}

void readLoginPacket(sf::Packet& packet, std::string& username, std::string& password, sf::Uint32& port)
{
    packet >> username >> password >> port;
}

void createDisconnectPacket(sf::Packet& packet)
{
    packet << Packet::Type::Disconnect;
}

void createClientMessagePacket(sf::Packet& packet, Message const& message)
{
    packet << Packet::Type::ClientMessage << message;
}

void readClientMessagePacket(sf::Packet& packet, Message& message)
{
    packet >> message;
}

} // namespace Packet
