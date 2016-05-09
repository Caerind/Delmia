#include "Forest.hpp"

Forest::Forest()
{
    mType = Buildings::Forest;
    initTile(sf::IntRect(512,0,256,256));
    addResource(Resources::Wood,1000);
}

Forest::Forest(sf::Vector2i const& coords) : Resource(coords)
{
    mType = Buildings::Forest;
    initTile(sf::IntRect(768,0,256,256));
    addResource(Resources::Wood,1000);
}
