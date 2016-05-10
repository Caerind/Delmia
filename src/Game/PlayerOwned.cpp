#include "PlayerOwned.hpp"

PlayerOwned::PlayerOwned(Player* player)
: mPlayer(player)
{
}

std::string PlayerOwned::getOwnerId() const
{
    if (mPlayer != nullptr)
    {
        return mPlayer->getId();
    }
    return "";
}
