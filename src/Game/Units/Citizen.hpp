#ifndef CITIZEN_HPP
#define CITIZEN_HPP

#include "Unit.hpp"

class Citizen : public Unit
{
    public:
        typedef std::shared_ptr<Citizen> Ptr;

    public:
        Citizen();
        Citizen(Player* player, sf::Vector2f const& pos);

    protected:
};

#endif // CITIZEN_HPP
