#include "Client.hpp"

Client::Client()
: on::Connection()
, on::Output(true)
, mThread(&Client::run,this)
, mMutex()
, mRunning(false)
{
    writeDebug("Client created");
    initPacketResponses();
}

Client::~Client()
{
    disconnect();
    writeDebug("Client destroyed");
}

bool Client::connect(sf::IpAddress const& address, sf::Uint16 port, std::string const& username, std::string const& password)
{
    if (!isConnected())
    {
        if (getSocketOut().connect(address,port,getTimeout()) == sf::Socket::Status::Done)
        {
            writeDebug("Connected to server");
            sf::TcpListener listener;
            if (listener.listen(sf::Socket::AnyPort) != sf::Socket::Status::Done)
            {
                writeDebug("Cant bind port : " + std::to_string(listener.getLocalPort()));
            }
            else
            {
                sf::Uint16 port = listener.getLocalPort();
                writeDebug("Listener opened on port : " + std::to_string(port));
                sf::Packet packet;
                std::string hashedPassword = password; // TODO : Hash password
                packet << Packet::Type::Login << port << username << hashedPassword;
                if (getSocketOut().send(packet) == sf::Socket::Done)
                {
                    writeDebug("Packet login sent");
                    if (listener.accept(getSocketIn()) == sf::Socket::Done)
                    {
                        writeDebug("Listener accepted server");
                        if (getSocketIn().receive(packet) == sf::Socket::Done)
                        {
                            writeDebug("Packet received from server");
                            sf::Int32 packetType;
                            packet >> packetType;
                            if (packetType == Packet::Type::LoginSuccess)
                            {
                                mUsername = username;
                                mPassword = hashedPassword;
                                mRunning = true;
                                mThread.launch();
                                return Connection::connect();
                            }
                        }
                    }
                }
            }
        }
    }
    return isConnected();
}

void Client::disconnect()
{
    on::Connection::disconnect();
    mThread.wait();
    writeDebug("Client disconnected");
}

void Client::handleCommand(std::string const& command)
{
    if (command == "stop")
    {
        writeDebug("Entered 'stop' command");
        sf::Packet packet;
        packet << Packet::Type::Disconnect;
        send(packet);
        disconnect();
    }
    else if (command != "")
    {
        writeDebug("Sending message : " + command);
        sf::Packet packet;
        packet << Packet::Type::Message << mUsername << command;
        send(packet);
    }
}

void Client::run()
{
    while (mRunning)
    {
        sf::Packet packet;
        while (poll(packet))
        {
            sf::Int32 packetType;
            packet >> packetType;

            auto itr = mPacketResponses.find(packetType);
            if (itr != mPacketResponses.end() && itr->second)
            {
                itr->second(packet);
            }
        }
    }
    disconnect();
}

void Client::initPacketResponses()
{
    mPacketResponses[Packet::Type::Message] = [&](sf::Packet& packet)
    {
        std::string username, message;
        packet >> username >> message;
        write(username + " : " + message);
    };

    mPacketResponses[Packet::Type::ServerStopped] = [&](sf::Packet& packet)
    {
        write("Server stopped");
        mRunning = false;
    };

    mPacketResponses[Packet::Type::ClientJoined] = [&](sf::Packet& packet)
    {
        std::string username;
        packet >> username;
        write(username + " joined the game");
    };

    mPacketResponses[Packet::Type::ClientLeft] = [&](sf::Packet& packet)
    {
        std::string username;
        packet >> username;
        write(username + " left the game");
    };
}
