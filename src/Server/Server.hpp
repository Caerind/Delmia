#ifndef ON_SERVER_HPP
#define ON_SERVER_HPP

// Standards Libs
#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <vector>

// SFML Network
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpListener.hpp>

// SFML System
#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Time.hpp>

// Own files
#include "Command.hpp"
#include "Peer.hpp"
#include "Message.hpp"

namespace on
{

template <typename T>
class Server
{
    public:
        typedef std::function<void(sf::Packet&,T&)> Response;

        Server(std::string const& logFile = "");
        ~Server();

        // Server
        virtual void load();
        virtual void start();
        virtual void stop();
        bool isRunning() const;

        // Packet
        void sendToAll(sf::Packet& packet, std::string const& excludeUser = "");
        void sendToPeer(sf::Packet& packet, std::string const& username);
        void sendToIp(sf::Packet& packet, sf::IpAddress const& ip);

        // Peer Management
        std::shared_ptr<T> getPeer(std::string const& username);
        bool isConnected(std::string const& username);

        // Commands
        std::string handleCommand(std::string const& command, bool server = true, std::string const& username = "[Server]");

    protected:
        void write(std::string const& message);

        virtual void onConnection(T& peer);
        virtual void onDisconnection(T& peer);

        void setListening(bool enable);

        void run();
        void update(sf::Time dt);

        void handlePackets();
        void handleConnections();
        void handleDisconnections();

    protected:
        sf::Thread mThread;
        bool mRunning;

        sf::TcpListener mListener;
        bool mListeningState;

        std::size_t mConnectedPlayers;
        std::vector<std::shared_ptr<T>> mPeers;

        std::ofstream mLog;

        std::size_t mMaxPlayers;
        std::size_t mPort;
        sf::Time mUpdateInterval;

        std::vector<std::string> mAdmins;
        std::vector<std::string> mBannedUsers;
        std::vector<sf::IpAddress> mBannedIps;

        std::map<std::string,Command> mCommands;
        std::map<sf::Int32,Response> mPacketResponses;

};

} // namespace

#include "Server.inl"

#endif // ON_SERVER_HPP
