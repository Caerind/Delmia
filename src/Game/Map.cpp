#include "Map.hpp"

#include "../Client.hpp"

Map::Map()
{
}

Map::~Map()
{
    for (std::size_t i = 0; i < mChunks.size(); i++)
    {
        if (!Client::isOnline())
        {
            mChunks[i]->saveToFile();
        }
        delete mChunks[i];
    }
    mChunks.clear();
}

sf::Vector2i Map::worldToChunk(sf::Vector2f const& pos)
{
    sf::Vector2i c;
    c.x = pos.x / (Chunk::getChunkSize().x * Chunk::getTileSize().x);
    if (pos.x < 0)
    {
        c.x--;
    }
    c.y = pos.y / (Chunk::getChunkSize().y * Chunk::getTileSize().y);
    if (pos.y < 0)
    {
        c.y--;
    }
    return c;
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
    /*

    sf::View& view = NWorld::getActiveView();
    sf::Vector2f delta = sf::Vector2f(50,50);
    sf::Vector2i begin = worldToChunk(view.getCenter() - view.getSize() * 0.5f - delta);
    sf::Vector2i end = worldToChunk(view.getCenter() + view.getSize() * 0.5f + delta);

    sf::Vector2i coords;
    std::vector<sf::Vector2i> usefullCoords;
    for (coords.x = begin.x; coords.x <= end.x; coords.x++)
    {
        for (coords.y = begin.y; coords.y <= end.y; coords.y++)
        {
            usefullCoords.push_back(coords);
        }
    }

    // Remove Useless Chunks
    for (std::size_t i = 0; i < mChunks.size();)
    {
        bool found = false;
        for (std::size_t j = 0; usefullCoords.size(); j++)
        {
            if (mChunks[i]->getCoords().x == usefullCoords[j].x)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            if (!Client::isOnline())
            {
                mChunks[i]->saveToFile();
            }
            delete mChunks[i];
            mChunks.erase(mChunks.begin() + i);
        }
        else
        {
            i++;
        }
    }

    // Load Usefull Chunks
    for (std::size_t i = 0; i < usefullCoords.size(); i++)
    {
        int x = usefullCoords[i].x;
        int y = usefullCoords[i].y;
        Chunk* c = getChunk(x,y);
        if (c == nullptr)
        {
            addChunk(x,y);
        }
    }

    */
}

void Map::addChunk(int cx, int cy)
{
    mChunks.push_back(new Chunk(sf::Vector2i(cx,cy)));
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
            delete mChunks[i];
            mChunks[i] = nullptr;
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
    Chunk* c = getChunk(cx,cy);
    if (c != nullptr)
    {
        c->setTileId(x,y,id);
    }
}

void Map::setTileId(int x, int y, int id)
{
    sf::Vector2i chunkCoords;
    sf::Vector2i tileCoords;
    chunkCoords = globalToChunk(sf::Vector2i(x,y));
    tileCoords = globalToRelative(sf::Vector2i(x,y));
    setTileId(chunkCoords.x,chunkCoords.y,tileCoords.x,tileCoords.y,id);
}

int Map::getTileId(int cx, int cy, int x, int y)
{
    Chunk* c = getChunk(cx,cy);
    if (c != nullptr)
    {
        return c->getTileId(x,y);
    }
    return 0;
}

int Map::getTileId(int x, int y)
{
    sf::Vector2i chunkCoords;
    sf::Vector2i tileCoords;
    chunkCoords = globalToChunk(sf::Vector2i(x,y));
    tileCoords = globalToRelative(sf::Vector2i(x,y));
    return getTileId(chunkCoords.x,chunkCoords.y,tileCoords.x,tileCoords.y);
}

void Map::load(pugi::xml_node& node)
{
}

void Map::save(pugi::xml_node& node)
{
    node.append_attribute("type") = "Map";
}

Chunk* Map::getChunk(int cx, int cy)
{
    sf::Vector2i coords = sf::Vector2i(cx,cy);
    for (std::size_t i = 0; i < mChunks.size(); i++)
    {
        if (mChunks[i]->getCoords() == coords)
        {
            return mChunks[i];
        }
    }
    return nullptr;
}
