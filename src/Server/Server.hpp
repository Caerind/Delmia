#ifndef ON_SERVER_HPP
#define ON_SERVER_HPP

#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <vector>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Time.hpp>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpListener.hpp>

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
        virtual void update(sf::Time dt);

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

} // namespace on

namespace on
{

template <typename T>
Server<T>::Server(std::string const& logFile)
: mThread(&Server::run,this)
, mRunning(false)
, mListener()
, mListeningState(false)
, mConnectedPlayers(0)
, mPeers(1)
, mLog(logFile)
, mMaxPlayers(10)
, mPort(4567)
, mUpdateInterval(sf::seconds(1.f/60.f))
{
    mListener.setBlocking(false);
	mPeers[0].reset(new T());
}

template <typename T>
Server<T>::~Server()
{
    stop();
}

template <typename T>
void Server<T>::load()
{
}

template <typename T>
void Server<T>::start()
{
    if (!mRunning)
    {
        sf::Clock clock;
        write("[Server] Loading server on port : " + std::to_string(mPort));

        load();

        std::ostringstream oss;
        oss << clock.restart().asSeconds();
        write("[Server] Loaded in " + oss.str() + "s !");

        mRunning = true;
        mThread.launch();

        write("[Server] Server started on ip : " + sf::IpAddress::getPublicAddress().toString() + " !");
    }
    else
    {
        write("[Server] Server is already running");
    }
}

template <typename T>
void Server<T>::stop()
{
    if (mRunning)
    {
        write("[Server] Stopping server");

        setListening(false);

        // TODO : Fix
        //sf::Packet packet;
        //packet << Packet::Type::MS_ServerStopped;
        //sendToAll(packet);

        mRunning = false;

        mThread.wait();

        write("[Server] Server stopped !");

        mLog.close();
    }
}

template <typename T>
bool Server<T>::isRunning() const
{
    return mRunning;
}

template <typename T>
void Server<T>::sendToAll(sf::Packet& packet, std::string const& excludeUser)
{
    for (std::size_t i = 0; i < mPeers.size(); i++)
    {
        if (mPeers[i]->isConnected() && mPeers[i]->getUsername() != excludeUser)
        {
            mPeers[i]->send(packet);
        }
    }
}

template <typename T>
void Server<T>::sendToPeer(sf::Packet& packet, std::string const& username)
{
    auto p = getPeer(username);
    if (p != nullptr)
    {
        if (p->isConnected())
        {
            p->send(packet);
        }
    }
}

template <typename T>
void Server<T>::sendToIp(sf::Packet& packet, sf::IpAddress const& ip)
{
    for (std::size_t i = 0; i < mPeers.size(); i++)
    {
        if (mPeers[i]->isConnected() && mPeers[i]->getRemoteAddress() == ip)
        {
            mPeers[i]->send(packet);
        }
    }
}

template <typename T>
std::shared_ptr<T> Server<T>::getPeer(std::string const& username)
{
    for (std::size_t i = 0; i < mPeers.size(); i++)
    {
        if (mPeers[i]->getUsername() == username)
        {
            return mPeers[i];
        }
    }
    return nullptr;
}

template <typename T>
bool Server<T>::isConnected(std::string const& username)
{
    auto p = getPeer(username);
    if (p != nullptr)
    {
        if (p->isConnected())
        {
            return true;
        }
    }
    return false;
}

template <typename T>
std::string Server<T>::handleCommand(std::string const& command, bool server, std::string const& username)
{
    std::vector<std::string> args = Command::getCommandName(command);
    if (args.size() >= 1)
    {
        auto itr = mCommands.find(args[0]);
        if (itr != mCommands.end())
        {
            if (server || !itr->second.isAdminOnly())
            {
                itr->second.execute(args[1],username,server);
            }
            else
            {
                return "You don't have the permission to do that";
            }
        }
        else
        {
            if (server)
            {
                write("[Server] Unknown command, try \"help\" for list of commands");
            }
            return "Unknown command";
        }
    }
    return "";
}

template <typename T>
void Server<T>::write(std::string const& message)
{
    // Get Time To Format
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,80,"[%x][%X]",timeinfo);

    // Write Message
    std::string str = std::string(buffer) + message + "\n";
    std::cout << str;
    if (mLog.is_open())
    {
        mLog << str;
    }
}

template <typename T>
void Server<T>::onConnection(T& peer)
{
}

template <typename T>
void Server<T>::onDisconnection(T& peer)
{
}

template <typename T>
void Server<T>::setListening(bool enable)
{
    if (enable) // Check if it isn't already listening
	{
		if (!mListeningState)
        {
            mListeningState = (mListener.listen(mPort) == sf::TcpListener::Done);
        }
	}
	else
	{
		mListener.close();
		mListeningState = false;
	}
}

template <typename T>
void Server<T>::run()
{
    setListening(true);

    sf::Clock updateClock;

    while (isRunning())
    {
        handlePackets();
        handleConnections();
        handleDisconnections();

        if (updateClock.getElapsedTime() > mUpdateInterval)
        {
            update(mUpdateInterval);
            updateClock.restart();
        }

        sf::sleep(sf::milliseconds(100.f));
    }
}

template <typename T>
void Server<T>::update(sf::Time dt)
{
}

template <typename T>
void Server<T>::handlePackets()
{
    for (std::size_t i = 0; i < mConnectedPlayers; i++)
	{
		if (mPeers[i]->isConnected())
        {
            sf::Packet packet;
            while (mPeers[i]->poll(packet))
            {
                sf::Int32 packetType;
                packet >> packetType;

                auto itr = mPacketResponses.find(packetType);
                if (itr != mPacketResponses.end() && itr->second)
                {
                    itr->second(packet,*mPeers[i]);
                }

                packet.clear();
            }
        }
	}
}

template <typename T>
void Server<T>::handleConnections()
{
    if (!mListeningState)
    {
        return;
    }

	if (mListener.accept(mPeers[mConnectedPlayers]->getSocketIn()) == sf::TcpListener::Done)
	{
		if (mPeers[mConnectedPlayers]->connect())
        {
            onConnection(*mPeers[mConnectedPlayers]);

            mConnectedPlayers++;

            if (mConnectedPlayers >= mMaxPlayers)
            {
                setListening(false);
            }
            else
            {
                mPeers.push_back(std::shared_ptr<T>(new T()));
            }
        }
	}
}

template <typename T>
void Server<T>::handleDisconnections()
{
    bool removePeer = false;

    for (std::size_t i = 0; i < mConnectedPlayers; i++)
	{
		if (mPeers[i]->needRemove())
		{
		    onDisconnection(*mPeers[i]);

			mPeers.erase(i + mPeers.begin());

			mConnectedPlayers--;

			removePeer = true;

			i--;
        }
	}

	if (removePeer)
    {
        if (mConnectedPlayers < mMaxPlayers)
        {
            mPeers.push_back(std::shared_ptr<T>(new T()));
            setListening(true);
        }
    }
}

} // namespace on

#endif // ON_SERVER_HPP
