#include "Building.hpp"

Building::Building()
: IsometricBase()
, PlayerOwned(nullptr)
, mType(Buildings::DefaultBuilding)
, mColor(sf::Color::White)
, mBuilt(false)
{
    mLife = 1.f;
    mLifeMax = 6.f;
}

Building::Building(Player* player, sf::Vector2i const& coords)
: IsometricBase(coords)
, PlayerOwned(player)
, mType(Buildings::DefaultBuilding)
, mColor(sf::Color::White)
, mBuilt(false)
{
    mLife = 1.f;
    mLifeMax = 6.f;
}

Building::~Building()
{
    clearTiles();
}

std::size_t Building::getType() const
{
    return mType;
}

sf::Color Building::getColor() const
{
    return mColor;
}

void Building::setColor(sf::Color const& color)
{
    mColor = color;
    for (std::size_t i = 0; i < mTiles.size(); i++)
    {
        mTiles[i].sprite.setColor(mColor);
    }
}

std::vector<sf::Vector2i> Building::getTilesBlueprint(sf::Vector2i const& coords)
{
    std::vector<sf::Vector2i> tiles;
    tiles.push_back(coords);
    return tiles;
}

/*
std::vector<std::pair<sf::Vector2i,sf::IntRect>> Building::getTiles(sf::Vector2i const& coords)
{
    std::vector<sf::Vector2i> tilesBP = Building::getTilesBlueprint(coords);
    std::vector<std::pair<sf::Vector2i,sf::IntRect>> tiles;
    for (std::size_t i = 0; i < tilesBP.size(); i++)
    {
        tiles.emplace_back();
        tiles.back().first = tilesBP[i];
    }
    // ADD YOUR TILES TEXTURE RECT
    return tiles;
}
*/

void Building::generate()
{
    clearTiles();

    std::vector<std::pair<sf::Vector2i,sf::IntRect>> tiles = getTiles(mCoords);
    for (std::size_t i = 0; i < tiles.size(); i++)
    {
        sf::IntRect rect;
        if (mBuilt)
        {
            rect = tiles[i].second;
        }
        else
        {
            rect = sf::IntRect(1536,0,256,256);
        }
        std::cout << rect.left << " " << mType << std::endl;
        addTile(tiles[i].first,"building",rect);
    }
}

void Building::generate(sf::Vector2i const& coords)
{
    mCoords = coords;
    generate();
}

void Building::setBuilt(bool built)
{
    if (mBuilt != built)
    {
        mBuilt = built;
        generate();

        if (mBuilt)
        {
            onBuildEnded();
        }
    }
}

void Building::build(float unit)
{
    if (!mBuilt)
    {
        mLife += unit;
        if (mLife >= mLifeMax)
        {
            mLife = mLifeMax;
            setBuilt(true);
        }
    }
}

void Building::onBuildEnded()
{
}

bool Building::isBuilt() const
{
    return mBuilt;
}

void Building::tick(sf::Time dt)
{
    build(dt.asSeconds());
}
