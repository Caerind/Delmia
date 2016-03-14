#include "Client.hpp"

namespace on
{

Client::Client()
: mThread(&Client::handlePackets,this)
{
    setTimeout(sf::seconds(3.f));
}

Client::~Client()
{
    disconnect();
}

void Client::handlePackets()
{
    while (isConnected())
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
            else
            {
                mUnhandledPackets.emplace();
                mUnhandledPackets.back().first = packetType;
                mUnhandledPackets.back().second = packet;
            }
        }
    }
}

bool Client::pollPacket(sf::Int32& packetType, sf::Packet& packet)
{
    if (mUnhandledPackets.size() >= 0)
    {
        packetType = mUnhandledPackets.front().first;
        packet = mUnhandledPackets.front().second;
        mUnhandledPackets.pop();
        return true;
    }
    return false;
}

sf::IpAddress Client::getRemoteAddress()
{
    return getSocketOut().getRemoteAddress();
}

std::string Client::getUsername() const
{
    return mUsername;
}

} // namespace on
