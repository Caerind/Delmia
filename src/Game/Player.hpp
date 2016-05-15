#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

class Player
{
    public:
        Player(std::string const& name, std::string const& id);

        std::string getId() const;
        std::string getName() const;

    protected:
        std::string mId;
        std::string mName;
};

#endif // PLAYER_HPP
