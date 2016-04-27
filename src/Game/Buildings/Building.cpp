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

void Building::generate(int x, int y)
{
    for (std::size_t i = 0; i < mTiles.size(); i++)
    {
        detachComponent(mTiles[i].sprite);
    }
    mTiles.clear();

    std::vector<std::pair<sf::Vector2i,sf::IntRect>> tiles = getTiles(x,y);
    for (std::size_t i = 0; i < tiles.size(); i++)
    {
        addTile(tiles[i].first.x,tiles[i].first.y,tiles[i].second);
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
