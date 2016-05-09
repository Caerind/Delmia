#include "Resource.hpp"

Resource::Resource()
: mType(Resources::DefaultResource)
, mCoords()
, mSprite()
{
}

Resource::Resource(sf::Vector2i const& coords)
: mType(Resources::DefaultResource)
, mCoords(coords)
, mSprite()
{
}

std::size_t Resource::getType() const
{
    return mType;
}

sf::Vector2i Resource::getCoords() const
{
    return mCoords;
}

bool Resource::collide(sf::Vector2i const& coords) const
{
    return (coords == mCoords);
}

void Resource::initTile(sf::IntRect const& textureRect)
{
    mSprite.setTexture("building");
    mSprite.setTextureRect(textureRect);
    mSprite.setOrigin(sf::Vector2f(textureRect.width * 0.5f, textureRect.height - 64.f));
    mSprite.setPosition(NIsometric::coordsToWorld(mCoords));
    mSprite.setPositionZ(1.f);
    attachComponent(&mSprite);
}
