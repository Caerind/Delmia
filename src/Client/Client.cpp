#include "Client.hpp"

Client* Client::mInstance = nullptr;

Client& Client::instance()
{
    if (mInstance == nullptr)
    {
        mInstance = new Client();
    }
    return *mInstance;
}

bool Client::connect(sf::IpAddress const& ip, sf::Uint32 port, std::string const& username, std::string const& password)
{
    if (!isConnected())
    {
        if (getSocketOut().connect(ip,port,getTimeout()) == sf::Socket::Status::Done)
        {
            int retry = 100;
            port += 10;
            sf::TcpListener listener;
            while (listener.listen(port) != sf::Socket::Status::Done && retry > 0)
            {
                ++port;
                --retry;
            }

            if (retry > 0)
            {
                sf::Packet packet;
                packet << Packet::Type::MS_Login << username << password << port;
                if (getSocketOut().send(packet) == sf::Socket::Done)
                {
                    if (listener.accept(getSocketIn()) == sf::Socket::Done)
                    {
                        mUsername = username;
                        mPassword = password;
                        mThread.launch();
                        return Connection::connect();
                    }
                }
            }
        }
    }
    return isConnected();
}

void Client::disconnect(bool sendPacket)
{
    if (isConnected())
    {
        if (sendPacket)
        {
            sf::Packet packet;
            packet << Packet::Type::MS_Disconnect;
            send(packet);
        }

        Connection::disconnect();

        mThread.wait();
    }
}

Client::Client() : on::Client()
{
    setTimeout(sf::seconds(3.f));

    mPacketResponses[Packet::Type::MS_ClientJoined] = [&](sf::Packet& packet)
    {
    };

    mPacketResponses[Packet::Type::MS_ClientLeft] = [&](sf::Packet& packet)
    {
    };

    mPacketResponses[Packet::Type::MS_ServerStopped] = [&](sf::Packet& packet)
    {
    };

    mPacketResponses[Packet::Type::MS_ServerMessage] = [&](sf::Packet& packet)
    {
        on::Message msg;
        packet >> msg;
        std::cout << msg << std::endl;
    };
}

Client::~Client()
{
    disconnect(true);
}
