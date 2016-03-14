#include "MainServer.hpp"

MainServer::MainServer() : on::Server<MainPeer>()
{
    mMaxPlayers = 50;
    mUpdateInterval = sf::seconds(1.f);

    initPacketResponses();
    initCommands();
}

void MainServer::initPacketResponses()
{
    mPacketResponses[Packet::Type::None] = [&](sf::Packet& packet, MainPeer& peer)
    {
    };

    mPacketResponses[Packet::Type::MS_Login] = [&](sf::Packet& packet, MainPeer& peer)
    {
    };

    mPacketResponses[Packet::Type::MS_Disconnect] = [&](sf::Packet& packet, MainPeer& peer)
    {
        peer.remove();
    };

    mPacketResponses[Packet::Type::MS_ClientMessage] = [&](sf::Packet& packet, MainPeer& peer)
    {
        on::Message msg;
        packet >> msg;
        packet.clear();

        if (msg.isCommand())
        {
            std::string res = handleCommand(msg.getContent().substr(1), false, peer.getUsername());
            if (res != "")
            {
                packet << Packet::Type::MS_ServerMessage << on::Message("",res);
                peer.send(packet);
            }
        }
        else
        {
            packet << Packet::Type::MS_ServerMessage << msg;
            sendToAll(packet);

            write(msg.getEmitter() + " : " + msg.getContent());
        }
    };
}

void MainServer::initCommands()
{
    mCommands["stop"] = on::Command("stop",[&](std::string const& args, std::string const& executant, bool isServer)
    {
        stop();
    });

    mCommands["help"] = on::Command("help",[&](std::string const& args, std::string const& executant, bool isServer)
    {
        if (isServer)
        {
            write("[Server] help : Display the list of commands");
            write("[Server] stop : Stop the server");
            write("[Server] say : Say something");
        }
        else
        {
            std::string str = "/help : Display the list of commands\n/say : Say something";
            sf::Packet packet;
            packet << Packet::Type::MS_ServerMessage << on::Message("[Server]",str);
            sendToPeer(packet,executant);
        }
    },false);

    mCommands["say"] = on::Command("say",[&](std::string const& args, std::string const& executant, bool isServer)
    {
        write("[Server] : " + args);

        sf::Packet packet;
        packet << Packet::Type::MS_ServerMessage << on::Message("[Server]",args);
        sendToAll(packet);
    });

    mCommands["list"] = on::Command("list",[&](std::string const& args, std::string const& executant, bool isServer)
    {
        if (isServer)
        {
            write("[Server] Listing users : ");
            for (std::size_t i = 0; i < mPeers.size(); i++)
            {
                if (mPeers[i]->isConnected() && mPeers[i]->getUsername() != "")
                {
                    write(" " + mPeers[i]->getUsername());
                }
            }
        }
    },false);
}

void MainServer::load()
{
    on::Server<MainPeer>::load();
}

void MainServer::start()
{
    on::Server<MainPeer>::start();
}

void MainServer::stop()
{
    on::Server<MainPeer>::stop();
}

void MainServer::update(sf::Time dt)
{
    on::Server<MainPeer>::update(dt);
}

void MainServer::onConnection(MainPeer& peer)
{
    write("[Server] " + peer.getUsername() + " joined the server");
}

void MainServer::onDisconnection(MainPeer& peer)
{
    write("[Server] " + peer.getUsername() + " left the server");
}
