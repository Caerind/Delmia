#include "Resource.hpp"

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
