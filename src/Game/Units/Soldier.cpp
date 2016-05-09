#include "Soldier.hpp"

Soldier::Soldier()
{
    mType = Units::Soldier;
}

Soldier::Soldier(sf::Vector2f const& pos) : Unit(pos)
{
    mType = Units::Soldier;
}
