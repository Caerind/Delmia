#include "Resource.hpp"
#include "World.hpp"

Resource::Resource()
: IsometricBase()
, mType(Resources::DefaultResource)
{
}

Resource::Resource(sf::Vector2i const& coords)
: IsometricBase(coords)
, mType(Resources::DefaultResource)
{
}

std::size_t Resource::getType() const
{
    return mType;
}

sf::Vector2f Resource::getEntryPoint() const
{
    sf::Vector2f center = NIsometric::coordsToWorld(mCoords);
    std::vector<sf::Vector2i> n = NIsometric::getNeighboors(mCoords,false);
    std::vector<sf::Vector2f> p;
    for (std::size_t i = 0; i < n.size(); i++)
    {
        if (!mWorld->collide(n[i]))
        {
            sf::Vector2f pos = NIsometric::coordsToWorld(n[i]);
            sf::Vector2f d = pos - center;
            setLength(d,100.f); // TODO : Fix size
            p.push_back(center + d);
        }
    }
    if (p.size() > 0)
    {
        return p[NMath::random(0,(int)p.size()-1)];
    }
    return sf::Vector2f();
}
