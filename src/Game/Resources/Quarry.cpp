#include "Quarry.hpp"

Quarry::Quarry()
{
    generateBuilding(0,0,sf::IntRect(256,0,256,256));
    addResource(Resources::rRock,1000);
}

Quarry::Quarry(int x, int y)
{
    generateBuilding(x,y,sf::IntRect(256,0,256,256));
    addResource(Resources::rRock,1000);
}
