#include "MainServer.hpp"

namespace MainServer
{

MainServer::MainServer()
: mRunning(false)
, mClients(Packet::Type::MS_Stopped,Packet::Type::MS_None,4567,sf::seconds(60.f))
, mServers(Packet::Type::MS_Stopped,Packet::Type::MS_None,4568,sf::seconds(60.f))
, mLog("")
{
    mCommands["stop"] = [&](std::string args, sf::IpAddress const& address, sf::Uint16 port)
    {
        stop();
    };

    mClients.setPacketResponse(Packet::Type::C_Disconnect,[&](sf::Packet& packet, sf::IpAddress const& address, sf::Uint16 port)
    {
        int id = mClients.getPeerId(address,port);
        if (id >= 0)
        {
            Client* c = mClients.getPeer(id);
            if (c != nullptr)
            {
                write(c->getUsername() + " left the server");
                c->setConnected(false);
            }
        }
    });

    mClients.setPacketResponse(Packet::Type::C_Login,[&](sf::Packet& packet, sf::IpAddress const& address, sf::Uint16 port)
    {
        int id = mClients.getPeerId(address,port);
        if (id < 0)
        {
            std::string username;
            std::string password;

            packet >> username >> password;

            bool valid = true; // TODO : DB checking
            if (valid)
            {
                // New client
                Client newClient;
                newClient.setAddress(address);
                newClient.setPort(port);
                newClient.setConnected(true);
                newClient.setUsername(username);
                mClients.addPeer(newClient);

                 // Send Login Valid
                packet.clear();
                packet << Packet::Type::MS_LoginValid;
                mClients.send(packet,address,port);

                // Send Server List
                packet.clear();
                packet << Packet::Type::MS_ServerList;
                // TODO : Server List
                mClients.send(packet,address,port);

                write(username + " joined the server");
            }
            else
            {
                // Send Login Failed
                packet.clear();
                packet << Packet::Type::MS_LoginFailed;
                mClients.send(packet,address,port);

                write("Login failed for client (" + address.toString() + ")");
            }
        }
    });

    mClients.setPacketResponse(Packet::Type::C_RequestServerList,[&](sf::Packet& packet, sf::IpAddress const& address, sf::Uint16 port)
    {
        packet.clear();
        packet << Packet::Type::MS_ServerList;
        // TODO : Server List
        mClients.send(packet,address,port);

        int id = mClients.getPeerId(address,port);
        if (id >= 0)
        {
            Client* c = mClients.getPeer(id);
            if (c != nullptr)
            {
                write(c->getUsername() + " requested server list");
            }
        }
    });

    mClients.setPacketResponse(Packet::Type::C_RequestGameServer,[&](sf::Packet& packet, sf::IpAddress const& address, sf::Uint16 port)
    {
        // TODO : Transfer User

        int id = mClients.getPeerId(address,port);
        if (id >= 0)
        {
            Client* c = mClients.getPeer(id);
            if (c != nullptr)
            {
                write(c->getUsername() + " requested to move to a game server");
            }
        }
    });

    mServers.setPacketResponse(Packet::Type::GS_Stopped,[&](sf::Packet& packet, sf::IpAddress const& address, sf::Uint16 port)
    {
        int id = mServers.getPeerId(address,port);
        if (id >= 0)
        {
            GameServer* gs = mServers.getPeer(id);
            if (gs != nullptr)
            {
                gs->setConnected(false);

                write("Server (" + address.toString() + ") disconnected");
            }
        }
    });

    mServers.setPacketResponse(Packet::Type::GS_Register,[&](sf::Packet& packet, sf::IpAddress const& address, sf::Uint16 port)
    {
        int id = mServers.getPeerId(address,port);
        if (id < 0)
        {
            GameServer newServer;
            newServer.setAddress(address);
            newServer.setPort(port);
            newServer.setConnected(true);
            mServers.addPeer(newServer);

            write("Server (" + address.toString() + ") joined");
        }
    });
}

MainServer::~MainServer()
{
    stop();
}

bool MainServer::start()
{
    if (!mRunning)
    {
        sf::Clock clock;
        write("Loading server...");

        // Run
        mRunning = true;

        // Ip
        write("Ip : " + sf::IpAddress::getPublicAddress().toString());

        // Client socket
        if (mClients.start())
        {
            write("Client Port : " + std::to_string(mClients.getPort()));
        }
        else
        {
            write("ERROR : Can't bind Client socket");
            stop();
            return false;
        }

        // Server socket
        if (mServers.start())
        {
            write("Server Port : " + std::to_string(mServers.getPort()));
        }
        else
        {
            write("ERROR : Can't bind Server socket");
            stop();
            return false;
        }

        // Loaded
        write("Server loaded in " + std::to_string(clock.restart().asSeconds()) + "s");
    }

    return mRunning;
}

void MainServer::stop()
{
    if (mRunning)
    {
        write("Stopping server...");

        mClients.stop();
        mServers.stop();

        mRunning = false;

        write("Server stopped");

        mLog.close();
    }
}

bool MainServer::isRunning() const
{
    return mRunning;
}

void MainServer::handleCommand(std::string command, sf::IpAddress const& address, sf::Uint16 port)
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

    mClients.handleCommand(command);
    mServers.handleCommand(command);
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

} // namespace MainServer
