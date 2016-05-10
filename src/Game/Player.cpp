#include "Player.hpp"

Player::Player(std::string const& name, std::string const& id)
: mId(id)
, mName(name)
{
}

std::string Player::getId() const
{
    return mId;
}

std::string Player::getName() const
{
    return mName;
}
