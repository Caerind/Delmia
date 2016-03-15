#ifndef MAINSERVER_SERVER_HPP
#define MAINSERVER_SERVER_HPP

#include <functional>
#include <map>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Time.hpp>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>

namespace MainServer
{

template <typename T>
class Server
{
    public:
        Server(sf::Int32 packetStop, sf::Int32 packetNone, sf::Uint16 port, sf::Time timeout);
        ~Server();

        bool start();
        void stop();
        bool isRunning() const;

        void handleCommand(std::string command, sf::IpAddress const& address = sf::IpAddress::LocalHost, sf::Uint16 port = 0);

        sf::IpAddress getAddress() const;
        sf::Uint16 getPort() const;
        sf::Time getTimeout() const;

        void addPeer(T const& peer);
        T* getPeer(std::size_t id);
        int getPeerId(sf::IpAddress const& address, sf::Uint16 port);
        std::size_t getPeerCount() const;

        void send(sf::Packet& packet, sf::IpAddress const& address, sf::Uint16 port);
        void sendToAll(sf::Packet& packet);

        typedef std::function<void(sf::Packet& packet, sf::IpAddress const& address, sf::Uint16 port)> Response;
        void setPacketResponse(sf::Int32 const& packetType, Response response);

        typedef std::function<void(std::string args, sf::IpAddress const& address, sf::Uint16 port)> Command;
        void setCommand(std::string const& commandName, Command command);

    protected:
        void run();

    protected:
        bool mRunning;
        sf::Thread mThread;
        sf::UdpSocket mSocket;
        std::vector<T> mPeers;
        std::map<sf::Int32,Response> mResponses;
        std::map<std::string,Command> mCommands;
        sf::Int32 mPacketStop;
        sf::Int32 mPacketNone;
        sf::Uint16 mPort;
        sf::Time mTimeout;
};

template <typename T>
Server<T>::Server(sf::Int32 packetStop, sf::Int32 packetNone, sf::Uint16 port, sf::Time timeout)
: mRunning(false)
, mThread(&Server<T>::run,this)
, mSocket()
, mPeers()
, mPacketStop(packetStop)
, mPacketNone(packetNone)
, mPort(port)
, mTimeout(timeout)
{
}

template <typename T>
Server<T>::~Server()
{
    stop();
}

template <typename T>
bool Server<T>::start()
{
    if (!mRunning)
    {
        if (mSocket.bind(mPort) == sf::Socket::Done)
        {
            mSocket.setBlocking(false);

            mRunning = true;
            mThread.launch();
        }
    }

    return mRunning;
}

template <typename T>
void Server<T>::stop()
{
    if (mRunning)
    {
        mSocket.setBlocking(true);

        sf::Packet packet;
        packet << mPacketStop;
        sendToAll(packet);

        mSocket.unbind();

        mRunning = false;

        mThread.wait();
    }
}

template <typename T>
bool Server<T>::isRunning() const
{
    return mRunning;
}

template <typename T>
void Server<T>::handleCommand(std::string command, sf::IpAddress const& address, sf::Uint16 port)
{
    std::string commandName;
    std::string commandArgs;

    std::size_t foundSpace = command.find_first_of(" ");
    if (foundSpace != std::string::npos)
    {
        commandName = command.substr(0, foundSpace);
        commandArgs = command.substr(foundSpace + 1);
    }
    else
    {
        commandName = command;
        commandArgs = "";
    }

    auto itr = mCommands.find(commandName);
    if (itr != mCommands.end())
    {
        if (itr->second)
        {
            itr->second(commandArgs,address,port);
        }
    }
}

template <typename T>
sf::IpAddress Server<T>::getAddress() const
{
    return sf::IpAddress::getPublicAddress();
}

template <typename T>
sf::Uint16 Server<T>::getPort() const
{
    return mPort;
}

template <typename T>
sf::Time Server<T>::getTimeout() const
{
    return mTimeout;
}

template <typename T>
void Server<T>::addPeer(T const& peer)
{
    mPeers.push_back(peer);
}

template <typename T>
T* Server<T>::getPeer(std::size_t id)
{
    if (id >= 0 && id < mPeers.size())
    {
        return &mPeers[id];
    }
    return nullptr;
}

template <typename T>
int Server<T>::getPeerId(sf::IpAddress const& address, sf::Uint16 port)
{
    for (std::size_t i = 0; i < mPeers.size(); i++)
    {
        if (mPeers[i].getAddress() == address && mPeers[i].getPort() == port)
        {
            return (int)i;
        }
    }
    return -1;
}

template <typename T>
std::size_t Server<T>::getPeerCount() const
{
    return mPeers.size();
}

template <typename T>
void Server<T>::send(sf::Packet& packet, sf::IpAddress const& address, sf::Uint16 port)
{
    mSocket.send(packet,address,port);
}

template <typename T>
void Server<T>::sendToAll(sf::Packet& packet)
{
    for (std::size_t i = 0; i < mPeers.size(); i++)
    {
        if (mSocket.send(packet,mPeers[i].getAddress(),mPeers[i].getPort()) == sf::Socket::Done)
        {
            mPeers[i].resetLastPacketTime();
        }
    }
}

template <typename T>
void Server<T>::setPacketResponse(sf::Int32 const& packetType, Response response)
{
    mResponses[packetType] = response;
}

template <typename T>
void Server<T>::setCommand(std::string const& commandName, Command command)
{
    mCommands[commandName] = command;
}

template <typename T>
void Server<T>::run()
{
    sf::Clock connectionClock;
    while (isRunning())
    {
        // Handle Packets
        sf::IpAddress address;
        sf::Uint16 port;
        sf::Packet packet;
        while (mSocket.receive(packet,address,port) == sf::Socket::Done)
        {
            sf::Int32 packetType;
            packet >> packetType;

            int id = getPeerId(address,port);
            if (id >= 0)
            {
                mPeers[id].resetLastPacketTime();
            }

            auto itr = mResponses.find(packetType);
            if (itr != mResponses.end())
            {
                if (itr->second)
                {
                    itr->second(packet,address,port);
                }
            }
        }

        // Handle Connection Checking
        if (connectionClock.getElapsedTime() >= 0.75f * mTimeout)
        {
            packet.clear();
            packet << mPacketNone;
            sendToAll(packet);
            connectionClock.restart();
        }

        // Handle Disconnections
        for (auto itr = mPeers.begin(); itr != mPeers.end();)
        {
            if (!(*itr).isConnected() || (*itr).getLastPacketTime() >= mTimeout)
            {
                itr = mPeers.erase(itr);
            }
            else
            {
                itr++;
            }
        }

        // Sleep
        sf::sleep(sf::milliseconds(100));
    }
}

} // namespace MainServer

#endif // MAINSERVER_SERVER_HPP
