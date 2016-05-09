#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

class Player
{
    public:
        Player();

        std::string getId() const;

    protected:
        std::string mId;
};

#endif // PLAYER_HPP
