#include "Server.hpp"

Server::Server()
: on::Output(true)
, mRunning(false)
, mListeningState(false)
, mListener()
, mThread(&Server::run,this)
, mMutex()
, mUpdateInterval(sf::seconds(1.f))
, mPort(4567)
, mConnectedPlayers(0)
, mPeers(1)
, mPacketResponses()
{
    mListener.setBlocking(false);
    mPeers[0].reset(new Peer());
    initPacketResponses();
}

Server::~Server()
{
    stop();
}

bool Server::start()
{
    if (!mRunning)
    {
        writeDebug("Starting");
        setListening(true);
        mRunning = true;
        mThread.launch();
        write("Started");
    }
    return mRunning;
}

void Server::stop()
{
    if (mRunning)
    {
        writeDebug("Stopping");
        setListening(false);
        sf::Packet packet;
        packet << Packet::Type::ServerStopped;
        sendToAll(packet);
        mRunning = false;
        mThread.wait();
        write("Stopped");
    }
}

bool Server::isRunning() const
{
    return mRunning;
}

void Server::sendToAll(sf::Packet& packet)
{
    for (std::size_t i = 0; i < mConnectedPlayers; i++)
    {
        if (mPeers[i]->isConnected())
        {
            mPeers[i]->send(packet);
        }
    }
}

void Server::handleCommand(std::string const& command)
{
    if (command == "stop")
    {
        stop();
    }
    else if (command == "list")
    {
        write("List : ");
        for (std::size_t i = 0; i < mConnectedPlayers; i++)
        {
            write(" " + std::to_string(i+1) + " - " + mPeers[i]->getUsername());
        }
    }
    else if (command != "")
    {
        std::string sender = "[Server]";
        sf::Packet packet;
        packet << Packet::Type::Message << sender << command;
        sendToAll(packet);
        write(sender + " : " + command);
    }
}

void Server::setListening(bool enable)
{
    if (enable)
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
	writeDebug("Listening connections : " + std::string((mListeningState) ? "yes":"no"));
}

void Server::run()
{
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

void Server::update(sf::Time dt)
{
}

void Server::handlePackets()
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
                    writeDebug("Handle Packet (" + std::to_string(packetType) + ")");
                }

                packet.clear();
            }
        }
	}
}

void Server::handleConnections()
{
    if (!mListeningState)
    {
        return;
    }

	if (mListener.accept(mPeers[mConnectedPlayers]->getSocketIn()) == sf::TcpListener::Done)
	{
		if (mPeers[mConnectedPlayers]->connect())
        {
            write(mPeers[mConnectedPlayers]->getUsername() + " joined the game");
            sf::Packet packet;
            packet << Packet::Type::ClientJoined << mPeers[mConnectedPlayers]->getUsername();
            sendToAll(packet);
            mConnectedPlayers++;
            mPeers.push_back(std::shared_ptr<Peer>(new Peer()));
        }
	}
}

void Server::handleDisconnections()
{
    for (std::size_t i = 0; i < mConnectedPlayers; i++)
	{
		if (mPeers[i]->needRemove() || !mPeers[i]->isConnected())
		{
            write(mPeers[i]->getUsername() + " left the game");
            sf::Packet packet;
            packet << Packet::Type::ClientLeft << mPeers[i]->getUsername();
            sendToAll(packet);
		    mPeers.erase(i + mPeers.begin());
			mConnectedPlayers--;
			i--;
        }
	}
}

void Server::initPacketResponses()
{
    mPacketResponses[Packet::Type::Message] = [&](sf::Packet& packet, Peer& peer)
    {
        std::string username;
        std::string message;
        packet >> username >> message;
        if (username == peer.getUsername())
        {
            write(peer.getUsername() +  " : " + message);
            packet.clear();
            packet << Packet::Type::Message << username << message;
            sendToAll(packet);
        }
    };

    mPacketResponses[Packet::Type::Disconnect] = [&](sf::Packet& packet, Peer& peer)
    {
        peer.remove();
    };
}
