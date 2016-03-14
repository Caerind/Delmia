#ifndef GAMESERVER_HPP
#define GAMESERVER_HPP

#include "../Server/Server.hpp"

#include "GamePeer.hpp"
#include "../Server/Message.hpp"
#include "../PacketType.hpp"

class GameServer : public Server<GamePeer>
{
    public:
        GameServer();

        // Server
        void load();
        void start();
        void stop();

        // Settings
        bool loadSettings();
        void saveSettings();
        void createSettings();

        // I/O
        void loadFromFile();
        void saveToFile();

        // Admin
        bool isAdmin(std::string const& username);
        void addAdmin(std::string const& username);
        void removeAdmin(std::string const& username);

        // Ban
        bool isBanned(std::string const& username);
        void ban(std::string const& username, std::string const& reason = "");
        void unban(std::string const& username);

        // Ban Ip
        bool isBannedIp(sf::IpAddress const& ip);
        void banIp(sf::IpAddress const& ip, std::string const& reason = "");
        void unbanIp(sf::IpAddress const& ip);

        // Kick
        void kick(std::string const& username, std::string const& reason = "");

    protected:
        void initPacketResponses();
        void initCommands();

        void onConnection(GamePeer& peer);
        void onDisconnection(GamePeer& peer);
};

#endif // GAMESERVER_HPP
