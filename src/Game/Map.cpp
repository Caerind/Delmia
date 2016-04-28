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

void Map::tick(sf::Time dt)
{
    // TODO : Remove useless chunks
    if (getChunkCount() > 20)
    {
        for (std::size_t i = 0; i < mChunks.size(); i++)
        {

        }
    }

    // TODO : Add usefull chunks
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
    // TODO : global to chunk and global to pos
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
    // TODO : global to chunk and global to pos
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
