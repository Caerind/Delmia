#include "Citizen.hpp"

Citizen::Citizen()
{
    mType = Units::Citizen;
}

Citizen::Citizen(Player* player, sf::Vector2f const& pos) : Unit(player, pos)
{
    mType = Units::Citizen;
}
