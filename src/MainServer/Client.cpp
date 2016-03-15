#include "Client.hpp"

namespace MainServer
{

Client::Client()
{
}

void Client::setUsername(std::string const& username)
{
    mUsername = username;
}

std::string Client::getUsername() const
{
    return mUsername;
}

} // namespace MainServer
