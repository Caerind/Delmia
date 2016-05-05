#include "Map.hpp"

#include "../Client.hpp"

Map::Map()
{
}

Map::~Map()
{
    if (!Client::isOnline())
    {
        for (std::size_t i = 0; i < mChunks.size(); i++)
        {
            mChunks[i]->saveToFile();
        }
    }
    mChunks.clear();
}

sf::Vector2i Map::worldToChunk(sf::Vector2f const& pos)
{
    return globalToChunk(NMapUtility::Isometric::worldToCoords(pos));
}

sf::Vector2i Map::globalToChunk(sf::Vector2i const& pos)
{
    sf::Vector2i c;
    c.x = pos.x / Chunk::getChunkSize().x;
    if (pos.x < 0)
    {
        c.x--;
    }
    c.y = pos.y / Chunk::getChunkSize().y;
    if (pos.y < 0)
    {
        c.y--;
    }
    return c;
}

sf::Vector2i Map::globalToRelative(sf::Vector2i const& pos)
{
    sf::Vector2i c;
    c.x  = pos.x % Chunk::getChunkSize().x;
    c.y  = pos.y % Chunk::getChunkSize().y;
    if (c.x < 0)
    {
        c.x += Chunk::getChunkSize().x;
    }
    if (c.y < 0)
    {
        c.y += Chunk::getChunkSize().y;
    }
    return c;
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
        mChunks.push_back(NWorld::createActor<Chunk>(coords));
        if (Client::isOnline())
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
                mChunks[i]->saveToFile();
            }
            NWorld::removeActor(mChunks[i]->getId());
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
            mChunks[i]->setTileId(x,y,id);
        }
    }
}

void Map::setTileId(int x, int y, int id)
{
    sf::Vector2i chunkCoords = globalToChunk(sf::Vector2i(x,y));
    sf::Vector2i tileCoords = globalToRelative(sf::Vector2i(x,y));
    setTileId(chunkCoords.x,chunkCoords.y,tileCoords.x,tileCoords.y,id);
}

int Map::getTileId(int cx, int cy, int x, int y)
{
    sf::Vector2i coords = sf::Vector2i(cx,cy);
    for (std::size_t i = 0; i < mChunks.size(); i++)
    {
        if (mChunks[i]->getCoords() == coords)
        {
            return mChunks[i]->getTileId(x,y);
        }
    }
    return 0;
}

int Map::getTileId(int x, int y)
{
    sf::Vector2i chunkCoords = globalToChunk(sf::Vector2i(x,y));
    sf::Vector2i tileCoords = globalToRelative(sf::Vector2i(x,y));
    return getTileId(chunkCoords.x,chunkCoords.y,tileCoords.x,tileCoords.y);
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
    sf::Vector2f delta = sf::Vector2f(Chunk::getTileSize().x,Chunk::getTileSize().y);
    sf::Vector2i begin = worldToChunk(view.getCenter() - view.getSize() * 0.5f - delta);
    sf::Vector2i end = worldToChunk(view.getCenter() + view.getSize() * 0.5f + delta);

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
