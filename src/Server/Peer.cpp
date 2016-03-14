#include "Peer.hpp"

namespace on
{

Peer::Peer()
: mRemove(false)
{
    setTimeout(sf::seconds(3.f));
}

Peer::~Peer()
{
    disconnect();
}

bool Peer::connect()
{
    return isConnected();
}

void Peer::remove()
{
    mRemove = true;
}

bool Peer::needRemove() const
{
    return mRemove;
}

sf::IpAddress Peer::getRemoteAddress()
{
    return getSocketIn().getRemoteAddress();
}

std::string Peer::getUsername() const
{
    return mUsername;
}

} // namespace on
