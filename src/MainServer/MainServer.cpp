#include "MainServer.hpp"

namespace MainServer
{

MainServer::MainServer()
: mRunning(false)
, mClientThread(&MainServer::runClient,this)
, mServerThread(&MainServer::runServer,this)
, mClients()
, mServers()
, mLog("")
, mClientPort(4567)
, mServerPort(4568)
, mClientTimeout(sf::seconds(60.f))
, mServerTimeout(sf::seconds(60.f))
{
}

MainServer::~MainServer()
{
    stop();
}

void MainServer::start()
{
    if (!mRunning)
    {
        sf::Clock clock;
        write("Loading server...");

        // Ip
        write("Ip : " + sf::IpAddress::getPublicAddress().toString());

        // Client socket
        if (mClientSocket.bind(mClientPort) == sf::Socket::Done)
        {
            write("Client Port : " + std::to_string(mClientPort));
        }
        else
        {
            write("ERROR : Unable to bind player socket");
        }

        // Server socket
        if (mServerSocket.bind(mServerPort) == sf::Socket::Done)
        {
            write("Server Port : " + std::to_string(mServerPort));
        }
        else
        {
            write("ERROR : Unable to bind server socket");
        }

        // Non-blocking sockets
        mClientSocket.setBlocking(false);
        mServerSocket.setBlocking(false);

        // Run
        mRunning = true;
        mClientThread.launch();
        mServerThread.launch();

        // Loaded
        write("Server loaded in " + std::to_string(clock.restart().asSeconds()) + "s");
    }
}

void MainServer::stop()
{
    if (mRunning)
    {
        write("Stopping server...");

        sf::Packet packet;
        packet << Packet::Type::MS_Stopped;
        sendToAllClients(packet);
        sendToAllServers(packet);

        mClientSocket.unbind();
        mServerSocket.unbind();

        mRunning = false;

        mClientThread.wait();
        mServerThread.wait();

        write("Server stopped");

        mLog.close();
    }
}

bool MainServer::isRunning() const
{
    return mRunning;
}

void MainServer::listClients()
{
    for (std::size_t i = 0; i < mClients.size(); i++)
    {
        write(" - " + mClients[i].getUsername() + " (" + mClients[i].getAddress().toString() + ":" + std::to_string(mClients[i].getPort()) + ")");
    }
}

void MainServer::listServers()
{
    for (std::size_t i = 0; i < mServers.size(); i++)
    {
        write(" - " + mServers[i].getAddress().toString() + ":" + std::to_string(mServers[i].getPort()));
    }
}

void MainServer::handleCommand(std::string const& command)
{
    if (command == "stop")
    {
        stop();
    }
    if (command == "clist")
    {
        listClients();
    }
    if (command == "slist")
    {
        listServers();
    }
}

void MainServer::sendToAllClients(sf::Packet& packet)
{
    for (std::size_t i = 0; i < mClients.size(); i++)
    {
        if (mClientSocket.send(packet,mClients[i].getAddress(),mClients[i].getPort()) == sf::Socket::Done)
        {
            mClients[i].resetLastPacketTime();
        }
    }
}

void MainServer::sendToAllServers(sf::Packet& packet)
{
    for (std::size_t i = 0; i < mServers.size(); i++)
    {
        if (mServerSocket.send(packet,mServers[i].getAddress(),mServers[i].getPort()) == sf::Socket::Done)
        {
            mServers[i].resetLastPacketTime();
        }
    }
}

void MainServer::write(std::string const& message)
{
    // Get Time To Format
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,80,"[%x][%X] ",timeinfo);

    // Write Message
    std::string str = std::string(buffer) + message;
    std::cout << str << std::endl;
    if (mLog.is_open())
    {
        mLog << str << std::endl;
    }
}

int MainServer::getClientId(sf::IpAddress address, sf::Uint16 port)
{
    for (std::size_t i = 0; i < mClients.size(); i++)
    {
        if (mClients[i].getAddress() == address && mClients[i].getPort() == port)
        {
            return (int)i;
        }
    }
    return -1;
}

int MainServer::getServerId(sf::IpAddress address, sf::Uint16 port)
{
    for (std::size_t i = 0; i < mServers.size(); i++)
    {
        if (mServers[i].getAddress() == address && mServers[i].getPort() == port)
        {
            return (int)i;
        }
    }
    return -1;
}

void MainServer::runClient()
{
    sf::Clock connectionClock;
    while (isRunning())
    {
        // Handle Packets
        sf::IpAddress address;
        sf::Uint16 port;
        sf::Packet packet;
        while (mClientSocket.receive(packet,address,port) == sf::Socket::Done)
        {
            sf::Int32 packetType;
            packet >> packetType;

            int id = getClientId(address,port);
            if (id >= 0) // Existing client
            {
                mClients[id].resetLastPacketTime();

                switch (packetType)
                {
                    case Packet::Type::C_Disconnect:
                    {
                        mClients[id].setConnected(false);
                    } break;

                    case Packet::Type::C_RequestServerList:
                    {
                        writeServerList(packet);
                        mClientSocket.send(packet,address,port);
                    } break;

                    case Packet::Type::C_RequestGameServer:
                    {
                        // TODO : Transfer User
                    } break;
                }
            }
            else // New client
            {
                if (packetType == Packet::Type::C_Login)
                {
                    std::string username, password;
                    packet >> username >> password;

                    // TODO : DB checking
                    bool valid = true;
                    if (valid)
                    {
                        // Add the client
                        Client newClient;
                        newClient.setAddress(address);
                        newClient.setPort(port);
                        newClient.setConnected(true);
                        newClient.setUsername(username);
                        mClients.push_back(newClient);

                        // Send Login Valid
                        packet.clear();
                        packet << Packet::Type::MS_LoginValid;
                        mClientSocket.send(packet,address,port);

                        // Send Server List
                        writeServerList(packet);
                        mClientSocket.send(packet,address,port);
                    }
                    else
                    {
                        // Send Login Failed
                        packet.clear();
                        packet << Packet::Type::MS_LoginFailed;
                        mClientSocket.send(packet,address,port);
                    }
                }
            }

            packet.clear();
        }

        // Handle Connection Checking
        if (connectionClock.getElapsedTime() >= 0.7f * mClientTimeout)
        {
            packet.clear();
            packet << Packet::Type::MS_None;
            sendToAllClients(packet);
            connectionClock.restart();
        }

        // Handle Disconnections
        for (auto itr = mClients.begin(); itr != mClients.end();)
        {
            if (!(*itr).isConnected() || (*itr).getLastPacketTime() >= mClientTimeout)
            {
                itr = mClients.erase(itr);
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

void MainServer::runServer()
{
    sf::Clock connectionClock;
    while (isRunning())
    {
        // Handle Packets
        sf::IpAddress address;
        sf::Uint16 port;
        sf::Packet packet;
        while (mServerSocket.receive(packet,address,port) == sf::Socket::Done)
        {
            sf::Int32 packetType;
            packet >> packetType;

            int id = getServerId(address,port);
            if (id >= 0) // Existing server
            {
                mServers[id].resetLastPacketTime();

                switch (packetType)
                {
                }
            }
            else // New server
            {
                if (packetType == Packet::Type::GS_Register)
                {
                    // Add the server
                    GameServer newServer;
                    newServer.setAddress(address);
                    newServer.setPort(port);
                    newServer.setConnected(true);
                    mServers.push_back(newServer);
                }
            }
        }

        // Handle Connection Checking
        if (connectionClock.getElapsedTime() >= 0.7f * mServerTimeout)
        {
            packet.clear();
            packet << Packet::Type::MS_None;
            sendToAllServers(packet);
            connectionClock.restart();
        }

        // Handle Disconnections
        for (auto itr = mServers.begin(); itr != mServers.end();)
        {
            if (!(*itr).isConnected() || (*itr).getLastPacketTime() >= mServerTimeout)
            {
                itr = mServers.erase(itr);
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

void MainServer::writeServerList(sf::Packet& packet)
{
    packet.clear();
    packet << Packet::Type::MS_ServerList;
    packet << mServers.size();
    for (std::size_t i = 0; i < mServers.size(); i++)
    {
        packet << mServers[i].getAddress().toString() << mServers[i].getPort();
    }
}

} // namespace MainServer
