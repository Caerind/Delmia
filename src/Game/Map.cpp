#include "Map.hpp"

Map::Map()
{
}

Map::~Map()
{
}

void Map::addChunk(sf::Vector2i const& coords)
{
    if (!contains(coords))
    {
        mChunks.push_back(new NIsometric::NLayerComponent("iso",coords));
        generate(mChunks.back());
        attachComponent(mChunks.back());
    }
}

void Map::tick(sf::Time dt)
{
    removeUselessChunks();
    addUsefullChunks();
}

void Map::generate(NIsometric::NLayerComponent* chunk)
{
    sf::Vector2i coords;
    for (coords.x = 0; coords.x < chunk->getLayerSize().x; coords.x++)
    {
        for (coords.y = 0; coords.y < chunk->getLayerSize().y; coords.y++)
        {
            chunk->setTileId(coords,1);

            if (coords.y + coords.x == 5)
            {
                chunk->setTileId(coords,2);
            }

            if (coords.y + coords.x == 10)
            {
                chunk->setTileId(coords,3);
            }
        }
    }
}

void Map::removeUselessChunks()
{
    std::vector<sf::Vector2i> u = determineUsefullChunks();
    std::vector<sf::Vector2i> r;

    for (std::size_t i = 0; i < mChunks.size();i++)
    {
        bool found = false;
        for (std::size_t j = 0; j < u.size(); j++)
        {
            if (u[j] == mChunks[i]->getCoords())
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            r.push_back(mChunks[i]->getCoords());
        }
    }
    for (std::size_t i = 0; i < r.size(); i++)
    {
        removeChunk(r[i]);
    }
}

void Map::addUsefullChunks()
{
    std::vector<sf::Vector2i> u = determineUsefullChunks();
    std::vector<sf::Vector2i> a;

    for (std::size_t i = 0; i < u.size(); i++)
    {
        bool found = false;
        for (std::size_t j = 0; j < mChunks.size(); j++)
        {
            if (u[i] == mChunks[j]->getCoords())
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            a.push_back(u[i]);
        }
    }

    for (std::size_t i = 0; i < a.size(); i++)
    {
        addChunk(a[i]);
    }
}

std::vector<sf::Vector2i> Map::determineUsefullChunks()
{
    std::vector<sf::Vector2i> usefull;

    sf::View& view = NWorld::getActiveView();
    sf::Vector2f delta = sf::Vector2f(mTileSize.x, mTileSize.y);
    sf::Vector2i begin = NIsometric::worldToChunk(view.getCenter() - view.getSize() * 0.5f - delta);
    sf::Vector2i end = NIsometric::worldToChunk(view.getCenter() + view.getSize() * 0.5f + delta);

    sf::Vector2i coords;
    for (coords.x = begin.x; coords.x <= end.x; coords.x++)
    {
        for (coords.y = begin.y; coords.y <= end.y; coords.y++)
        {
            usefull.push_back(coords);
        }
    }

    return usefull;
}
