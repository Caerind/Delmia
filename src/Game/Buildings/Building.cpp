#include "Building.hpp"

Building::Building()
{
}

Building::~Building()
{
    for (std::size_t i = 0; i < mTiles.size(); i++)
    {
        delete mTiles[i].sprite;
    }
    mTiles.clear();
}

std::vector<sf::Vector2i> Building::getTilesBlueprint(int x, int y)
{
    std::vector<sf::Vector2i> tiles;
    tiles.push_back(sf::Vector2i(x,y));
    return tiles;
}

std::vector<sf::Vector2i> Building::getTiles(int x, int y)
{
    return Building::getTilesBlueprint(x,y);
}

void Building::generateBuilding(int x, int y, sf::IntRect rect)
{
    std::vector<sf::Vector2i> tiles = getTiles(x,y);
    for (std::size_t i = 0; i < tiles.size(); i++)
    {
        addTile(tiles[i].x,tiles[i].y,rect);
    }
}

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

    attachComponent(mTiles.back().sprite);
}

sf::FloatRect Building::getBounds()
{
    // TODO : Bounds
    for (std::size_t i = 0; i < mTiles.size(); i++)
    {
        return mTiles[i].sprite->getBounds();
    }
    return sf::FloatRect();
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
