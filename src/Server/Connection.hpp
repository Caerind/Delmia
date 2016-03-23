#ifndef ON_CONNECTION_HPP
#define ON_CONNECTION_HPP

#include <queue>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Time.hpp>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpSocket.hpp>

namespace on
{

class Connection
{
    public:
        Connection();
        virtual ~Connection();

        bool isConnected() const;
        bool connect();
        virtual void disconnect();

        bool poll(sf::Packet& packet);
        void send(sf::Packet& packet);

        sf::Uint32 getId() const;

        sf::TcpSocket& getSocketIn();
        sf::TcpSocket& getSocketOut();

        void setTimeout(sf::Time const& timeout);
        sf::Time getTimeout() const;

    private:
        bool mConnected;

        void receive();
        sf::TcpSocket mSocketIn;
        sf::Thread mReceiveThread;
        sf::Mutex mReceiveMutex;
        std::queue<sf::Packet> mIncoming;

        void send();
        sf::TcpSocket mSocketOut;
        sf::Thread mSendThread;
        sf::Mutex mSendMutex;
        std::queue<sf::Packet> mOutgoing;

        static sf::Uint32 mNumberOfCreations;
        const sf::Uint32 mId;

        sf::Time mTimeout;
};

} // namespace on

#endif // ON_CONNECTION_HPP
