#include "Building.hpp"

Building::Building()
: mType(Buildings::DefaultBuilding)
, mCoords(sf::Vector2i(0,0))
, mColor(sf::Color::White)
, mTiles()
, mBuilt(false)
{
    mLife = 1.f;
    mLifeMax = 6.f;
}

Building::Building(int x, int y)
: mCoords(sf::Vector2i(x,y))
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

std::vector<sf::Vector2i> Building::getTilesBlueprint(int x, int y)
{
    std::vector<sf::Vector2i> tiles;
    tiles.push_back(sf::Vector2i(x,y));
    return tiles;
}

/*
std::vector<std::pair<sf::Vector2i,sf::IntRect>> Building::getTiles(int x, int y)
{
    std::vector<sf::Vector2i> tilesBP = Building::getTilesBlueprint(x,y);
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

void Building::addTile(int x, int y, sf::IntRect rect)
{
    sf::Vector2i coords = sf::Vector2i(x,y);

    mTiles.emplace_back();
    mTiles.back().coords = coords;
    mTiles.back().sprite = new NSpriteComponent();
    mTiles.back().sprite->setTexture("building",rect);
    mTiles.back().sprite->setOrigin(rect.width/2,rect.height-64);
    mTiles.back().sprite->setPosition(NMapUtility::Isometric::coordsToWorld(coords));
    mTiles.back().sprite->setPositionZ(1.f);
    mTiles.back().sprite->setColor(mColor);

    attachComponent(mTiles.back().sprite);
}

void Building::generate()
{
    clearTiles();

    std::vector<std::pair<sf::Vector2i,sf::IntRect>> tiles = getTiles(mCoords.x,mCoords.y);
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
        addTile(tiles[i].first.x,tiles[i].first.y,rect);
    }
}

void Building::generate(int x, int y)
{
    setCoords(x,y);
    generate();
}

bool Building::collide(int x, int y)
{
    sf::Vector2i coords = sf::Vector2i(x,y);
    for (std::size_t i = 0; i < mTiles.size(); i++)
    {
        if (mTiles[i].coords == coords)
        {
            return true;
        }
    }
    return false;
}

void Building::load(pugi::xml_node& node)
{
}

void Building::save(pugi::xml_node& node)
{
}

void Building::setCoords(int x, int y)
{
    mCoords = sf::Vector2i(x,y);
}

sf::Vector2i Building::getCoords() const
{
    return mCoords;
}

void Building::setColor(sf::Color color)
{
    mColor = color;
    for (std::size_t i = 0; i < mTiles.size(); i++)
    {
        mTiles[i].sprite->setColor(mColor);
    }
}

sf::Color Building::getColor() const
{
    return mColor;
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
