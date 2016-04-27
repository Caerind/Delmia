#include "GoldMine.hpp"

GoldMine::GoldMine()
{
    generateBuilding(0,0,sf::IntRect(256,0,256,256));
    addResource(Resources::Gold,1000);
}

GoldMine::GoldMine(int x, int y)
{
    generateBuilding(x,y,sf::IntRect(256,0,256,256));
    addResource(Resources::Gold,1000);
}
