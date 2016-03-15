#ifndef MAINSERVER_SERVER_HPP
#define MAINSERVER_SERVER_HPP

namespace MainServer
{

template <typename T>
class Server
{
    public:
        Server();
        Server(sf::Uint16 port, sf::Time timeout = sf::seconds(10.f));
        ~Server();

        void start();
        void stop();
        bool isRunning() const;

        void handleCommand(std::string command, sf::IpAddress const& address = sf::IpAddress::LocalHost, sf::Uint16 port = 0);

    protected:
        void sendToAll(sf::Packet& packet);

        int getPeerId(sf::IpAddress const& address, sf::Uint16 port);

        void run();

        typedef std::function<void(sf::Packet& packet, sf::IpAddress const& address, sf::Uint16 port)> Response;
        void setPacketResponse(sf::Int32 const& packetType, Response response);

        typedef std::function<void(std::string args, sf::IpAddress const& address, sf::Uint16 port)> Command;
        void setCommand(std::string const& commandName, Command command);

        void initDefaultResponses();
        void initDefaultCommands();

    protected:
        bool mRunning;
        sf::Thread mThread;
        sf::UdpSocket mSocket;
        sf::Uint16 mPort;
        sf::Time mTimeout;
        std::vector<T> mPeers;
        std::map<sf::Int32,Response> mResponses;
        std::map<std::string,Command> mCommands;
};

template <typename T>
Server<T>::Server()
: mRunning(false)
, mThread(&Server<T>::run,this)
, mSocket()
, mPort(4567)
, mTimeout(sf::seconds(10.f))
, mPeers()
{
    initDefaultResponses();
    initDefaultCommands();
}

template <typename T>
Server<T>::Server(sf::Uint16 port, sf::Time timeout)
: mRunning(false)
, mThread(&Server<T>::run,this)
, mSocket()
, mPort(port)
, mTimeout(timeout)
, mPeers()
{
    initDefaultResponses();
    initDefaultCommands();
}

template <typename T>
Server<T>::~Server()
{
    stop();
}

template <typename T>
void Server<T>::start()
{
    if (!mRunning)
    {
        mSocket.bind(mPort);
        mSocket.setBlocking(false);

        mRunning = true;
        mThread.launch();
    }
}

template <typename T>
void Server<T>::stop()
{
    if (mRunning)
    {
        mSocket.setBlocking(true);

        sf::Packet packet;
        sf::Int32 packetType = 777888999;
        packet << packetType; // Mean the server stop
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
void Server<T>::sendToAll(sf::Packet& packet)
{
    for (std::size_t i = 0; i < mPeers.size(); i++)
    {
        if (mSocket.send(packet,mmPeers[i].getAddress(),mPeers[i].getPort()) == sf::Socket::Done)
        {
            mPeers[i].resetLastPacketTime();
        }
    }
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
            if (id >= 0) // Existing client
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

            packet.clear();
        }

        // Handle Connection Checking
        if (connectionClock.getElapsedTime() >= 0.7f * mTimeout)
        {
            packet.clear();
            sf::Int32 packetType = 888777999;
            packet << packetType;
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

template <typename T>
void Server<T>::setPacketResponse(sf::Int32 packetType, Response response)
{
    mResponses[packetType] = response;
}

template <typename T>
void Server<T>::setCommand(std::string const& commandName, Command command)
{
    mCommands[commandName] = command;
}

template <typename T>
void Server<T>::initDefaultResponses()
{
}

template <typename T>
void Server<T>::initDefaultCommands()
{
}

} // namespace MainServer

#endif // MAINSERVER_SERVER_HPP
