#include "Forest.hpp"

Forest::Forest()
{
    generateBuilding(0,0,sf::IntRect(512,0,256,256));
    addResource(Resources::rWood,1000);
}

Forest::Forest(int x, int y)
{
    generateBuilding(x,y,sf::IntRect(512,0,256,256));
    addResource(Resources::rWood,1000);
}
