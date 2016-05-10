#include "IsometricBase.hpp"

IsometricBase::IsometricBase(sf::Vector2i const& coords)
{
    mCoords = coords;
}

IsometricBase::~IsometricBase()
{
    clearTiles();
}

void IsometricBase::setCoords(sf::Vector2i const& coords)
{
    mCoords = coords;
    clearTiles();
}

sf::Vector2i IsometricBase::getCoords() const
{
    return mCoords;
}

void IsometricBase::addTile(sf::Vector2i const& coords, std::string const& textureId, sf::IntRect const& textureRect)
{
    mTiles.emplace_back();
    mTiles.back().coords = coords;
    mTiles.back().sprite.setTexture(textureId,textureRect);
    mTiles.back().sprite.setOrigin(sf::Vector2f(textureRect.width * 0.5f, textureRect.height - 64.f));
    mTiles.back().sprite.setPosition(NIsometric::coordsToWorld(coords));
    mTiles.back().sprite.setPositionZ(1.f);
}

void IsometricBase::removeTile(sf::Vector2i const& coords)
{
    for (std::size_t i = 0; i < mTiles.size();)
    {
        if (mTiles[i].coords == coords)
        {
            erase(mTiles,i);
        }
        else
        {
            i++;
        }
    }
}

void IsometricBase::clearTiles()
{
    mTiles.clear();
}

bool IsometricBase::hasTile(sf::Vector2i const& coords)
{
    for (std::size_t i = 0; i < mTiles.size(); i++)
    {
        if (mTiles[i].coords == coords)
        {
            return true;
        }
    }
    return false;
}

sf::Vector2f IsometricBase::getEntryPoint() const
{
    return sf::Vector2f();
}
