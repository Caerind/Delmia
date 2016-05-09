#include "GoldMine.hpp"

GoldMine::GoldMine()
{
    mType = Buildings::GoldMine;
    initTile(sf::IntRect(256,0,256,256));
    addResource(Resources::Gold,1000);
}

GoldMine::GoldMine(sf::Vector2i const& coords) : Resource(coords)
{
    mType = Buildings::GoldMine;
    initTile(sf::IntRect(256,0,256,256));
    addResource(Resources::Gold,1000);
}
