#include "Map.hpp"

#include "../Client.hpp"

Map::Map()
{
    NIsometric::setLayerSize(sf::Vector2i(16,64));
    NIsometric::setTileSize(sf::Vector2i(256,128));
}

Map::~Map()
{
    if (!Client::isOnline())
    {
        for (std::size_t i = 0; i < mChunks.size(); i++)
        {
            //mChunks[i]->saveToFile();
        }
    }
    mChunks.clear();
}

void Map::tick(sf::Time dt)
{
    removeUselessChunks();
    addUsefullChunks();
}

void Map::addChunk(int cx, int cy)
{
    sf::Vector2i coords = sf::Vector2i(cx,cy);
    if (!contains(coords))
    {
        mChunks.push_back(new NIsometric::NLayerComponent("iso",coords));
        /*if (Client::isOnline())
        {
            // TODO : Load from online
        }
        else
        {
            if (!mChunks.back()->loadFromFile())
            {
                mChunks.back()->generate();
            }
        }
        */
        mChunks.back()->setPositionZ(-10.f);
        generate(mChunks.back());
    }
}

void Map::removeChunk(int cx, int cy)
{
    sf::Vector2i coords = sf::Vector2i(cx,cy);
    for (std::size_t i = 0; i < mChunks.size();)
    {
        if (mChunks[i]->getCoords() == coords)
        {
            if (!Client::isOnline())
            {
                //mChunks[i]->saveToFile();
            }
            delete mChunks[i];
            mChunks.erase(mChunks.begin() + i);
        }
        else
        {
            i++;
        }
    }
}

std::size_t Map::getChunkCount() const
{
    return mChunks.size();
}

void Map::setTileId(int cx, int cy, int x, int y, int id)
{
    sf::Vector2i coords = sf::Vector2i(cx,cy);
    for (std::size_t i = 0; i < mChunks.size(); i++)
    {
        if (mChunks[i]->getCoords() == coords)
        {
            mChunks[i]->setTileId({x,y},id);
        }
    }
}

void Map::setTileId(int x, int y, int id)
{
    sf::Vector2i chunkCoords = NIsometric::coordsToChunk(sf::Vector2i(x,y));
    sf::Vector2i tileCoords = NIsometric::coordsToRelative(sf::Vector2i(x,y));
    setTileId(chunkCoords.x,chunkCoords.y,tileCoords.x,tileCoords.y,id);
}

int Map::getTileId(int cx, int cy, int x, int y)
{
    sf::Vector2i coords = sf::Vector2i(cx,cy);
    for (std::size_t i = 0; i < mChunks.size(); i++)
    {
        if (mChunks[i]->getCoords() == coords)
        {
            return mChunks[i]->getTileId({x,y});
        }
    }
    return 0;
}

int Map::getTileId(int x, int y)
{
    sf::Vector2i chunkCoords = NIsometric::coordsToChunk(sf::Vector2i(x,y));
    sf::Vector2i tileCoords = NIsometric::coordsToRelative(sf::Vector2i(x,y));
    return getTileId(chunkCoords.x,chunkCoords.y,tileCoords.x,tileCoords.y);
}

void Map::generate(NIsometric::NLayerComponent* chunk)
{
    sf::Vector2i coords;
    for (coords.x = 0; coords.x < chunk->getLayerSize().x; coords.x++)
    {
        for (coords.y = 0; coords.y < chunk->getLayerSize().y; coords.y++)
        {
            chunk->setTileId(coords,Tile::Dirt);

            if (coords.y + coords.x == 5)
            {
                chunk->setTileId(coords,Tile::Path);
            }

            if (coords.y + coords.x == 10)
            {
                chunk->setTileId(coords,Tile::Water);
            }
        }
    }
}

void Map::load(pugi::xml_node& node)
{
}

void Map::save(pugi::xml_node& node)
{
    node.append_attribute("type") = "Map";
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
        removeChunk(r[i].x,r[i].y);
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
        addChunk(a[i].x,a[i].y);
    }
}

bool Map::contains(sf::Vector2i const& coords) const
{
    for (std::size_t i = 0; i < mChunks.size(); i++)
    {
        if (mChunks[i]->getCoords() == coords)
        {
            return true;
        }
    }
    return false;
}

std::vector<sf::Vector2i> Map::determineUsefullChunks()
{
    std::vector<sf::Vector2i> usefull;

    sf::View& view = NWorld::getActiveView();
    sf::Vector2f delta = sf::Vector2f(NIsometric::getTileSize().x, NIsometric::getTileSize().y);
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
