#include "Forest.hpp"

Forest::Forest()
{
    setCoords(0,0);
    addSprite(0,0,sf::IntRect(512,0,256,256));
    addResource(0,1000); // TODO : Resources
}

Forest::Forest(int x, int y)
{
    setCoords(x,y);
    addSprite(0,0,sf::IntRect(512,0,256,256));
    addResource(0,1000); // TODO : Resources
}
