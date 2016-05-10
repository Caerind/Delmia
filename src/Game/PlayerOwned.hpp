#ifndef PLAYEROWNED_HPP
#define PLAYEROWNED_HPP

#include <string>

#include "Player.hpp"

class PlayerOwned
{
    public:
        PlayerOwned(Player* player = nullptr);

        std::string getOwnerId() const;

    protected:
        Player* mPlayer;
};

#endif // PLAYEROWNED_HPP
