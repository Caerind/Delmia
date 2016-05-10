#include "Quarry.hpp"

Quarry::Quarry()
{
    mType = Buildings::Quarry;
}

Quarry::Quarry(sf::Vector2i const& coords) : Resource(coords)
{
    mType = Buildings::Quarry;
    addTile(coords,"building",sf::IntRect(1024,0,256,256));
    addResource(Resources::Rock,1000);
}
