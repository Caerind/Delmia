#include "Player.hpp"

Player::Player()
{
    mId = "ImThePlayerOne";
}

std::string Player::getId() const
{
    return mId;
}
