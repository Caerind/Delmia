#include "Resource.hpp"

Resource::Resource()
{
    mCoords.x = 0;
    mCoords.y = 0;
}

Resource::Resource(int x, int y)
{
    mCoords.x = x;
    mCoords.y = y;
}

sf::Vector2i Resource::getCoords() const
{
    return mCoords;
}
