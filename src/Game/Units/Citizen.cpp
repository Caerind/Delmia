#include "Citizen.hpp"

Citizen::Citizen()
{
    mType = Units::Citizen;
}

Citizen::Citizen(sf::Vector2f const& pos) : Unit(pos)
{
    mType = Units::Citizen;
}
