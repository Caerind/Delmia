#include "Forest.hpp"

Forest::Forest()
{
    mType = Buildings::Forest;
}

Forest::Forest(sf::Vector2i const& coords) : Resource(coords)
{
    mType = Buildings::Forest;
    addTile(coords,"building",sf::IntRect(512,0,256,256));
    addResource(Resources::Wood,1000);
}
