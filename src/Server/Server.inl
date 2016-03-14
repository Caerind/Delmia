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
        write("[Server] Loading server");

        load();

        std::ostringstream oss;
        oss << clock.restart().asSeconds();
        write("[Server] Loaded in " + oss.str() + "s !");

        mRunning = true;
        mThread.launch();

        write("[Server] Server started !");
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

        // TODO : Fix it
        /*
        sf::Packet packet;
        Packet::createServerStoppedPacket(packet);
        sendToAll(packet);
        */

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
