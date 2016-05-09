#include "Building.hpp"

Building::Building()
: mType(Buildings::DefaultBuilding)
, mCoords()
, mColor(sf::Color::White)
, mTiles()
, mBuilt(false)
{
    mLife = 1.f;
    mLifeMax = 6.f;
}

Building::Building(sf::Vector2i const& coords)
: mType(Buildings::DefaultBuilding)
, mCoords(coords)
, mColor(sf::Color::White)
, mTiles()
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
sf::Vector2i Building::getCoords() const
{
    return mCoords;
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
        mTiles[i].sprite->setColor(mColor);
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

void Building::addTile(sf::Vector2i const& coords, sf::IntRect rect)
{
    mTiles.emplace_back();
    mTiles.back().coords = coords;
    mTiles.back().sprite = new NSpriteComponent();
    mTiles.back().sprite->setTexture("building",rect);
    mTiles.back().sprite->setOrigin({rect.width * 0.5f, rect.height - 64.f});
    mTiles.back().sprite->setPosition(NIsometric::coordsToWorld(coords));
    mTiles.back().sprite->setPositionZ(1.f);
    mTiles.back().sprite->setColor(mColor);

    attachComponent(mTiles.back().sprite);
}

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
        addTile(tiles[i].first,rect);
    }
}

void Building::generate(sf::Vector2i const& coords)
{
    mCoords = coords;
    generate();
}

bool Building::collide(sf::Vector2i const& coords)
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

void Building::clearTiles()
{
    for (std::size_t i = 0; i < mTiles.size(); i++)
    {
        detachComponent(mTiles[i].sprite);
        delete mTiles[i].sprite;
    }
    mTiles.clear();
}

void Building::tick(sf::Time dt)
{
    build(dt.asSeconds());
}

void Building::load(pugi::xml_node& node)
{
}

void Building::save(pugi::xml_node& node)
{
}
