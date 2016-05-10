#include "Soldier.hpp"

Soldier::Soldier()
{
    mType = Units::Soldier;
}

Soldier::Soldier(Player* player, sf::Vector2f const& pos) : Unit(player, pos)
{
    mType = Units::Soldier;
}
