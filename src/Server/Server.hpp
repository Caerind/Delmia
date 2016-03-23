#ifndef SERVER_HPP
#define SERVER_HPP

#include <functional>
#include <map>
#include <memory>
#include <vector>

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "PacketType.hpp"
#include "Peer.hpp"
#include "Output.hpp"

class Server : public on::Output
{
    public:
        typedef std::function<void(sf::Packet&,Peer&)> Response;

        Server();
        ~Server();

        bool start();
        void stop();
        bool isRunning() const;

        void sendToAll(sf::Packet& packet);

        void handleCommand(std::string const& command);

    protected:
        void setListening(bool enable);
        void run();
        void update(sf::Time dt);
        void handlePackets();
        void handleConnections();
        void handleDisconnections();

        void initPacketResponses();

    protected:
        bool mRunning;
        bool mListeningState;
        sf::TcpListener mListener;
        sf::Thread mThread;
        sf::Mutex mMutex;
        sf::Time mUpdateInterval;
        sf::Uint16 mPort;
        std::size_t mConnectedPlayers;
        std::vector<std::shared_ptr<Peer>> mPeers;
        std::map<sf::Int32,Response> mPacketResponses;
};

#endif // SERVER_HPP
