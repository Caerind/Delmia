#include "Map.hpp"

Map::Map()
{
}

void Map::tick(sf::Time dt)
{
    // TODO : Remove useless chunks

    // TODO : Add usefull chunks
}

void Map::addChunk(int cx, int cy)
{
    mChunks.push_back(new Chunk());
    mChunks.back()->layer.create("iso",getChunkSize(),getTileSize(),NLayerComponent::Isometric);
    mChunks.back()->layer.setPosition(getChunkSize().x * getTileSize().x * cx, getChunkSize().y * getTileSize().y * cy,-10.f);
    mChunks.back()->coords = sf::Vector2i(cx,cy);

    attachComponent(&mChunks.back()->layer);

    sf::Vector2i coords;
    for (coords.x = 0; coords.x < getChunkSize().x; coords.x++)
    {
        for (coords.y = 0; coords.y < getChunkSize().y; coords.y++)
        {
            mChunks.back()->layer.setTileId(coords,Tile::Dirt);
        }
    }
}

void Map::removeChunk(int cx, int cy)
{
    sf::Vector2i coords = sf::Vector2i(cx,cy);
    for (std::size_t i = 0; i < mChunks.size();)
    {
        if (mChunks[i]->coords == coords)
        {
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

sf::Vector2i Map::getChunkSize()
{
    return sf::Vector2i(16,64);
}

sf::Vector2i Map::getTileSize()
{
    return sf::Vector2i(256,128);
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
        c->layer.setTileId(sf::Vector2i(x,y),id);
    }
}

void Map::setTileId(int x, int y, int id)
{
    // TODO : global to chunk and global to pos
}

int Map::getTileId(int cx, int cy, int x, int y)
{
    Chunk* c = getChunk(cx,cy);
    if (c != nullptr)
    {
        return c->layer.getTileId(sf::Vector2i(x,y));
    }
    return 0;
}

int Map::getTileId(int x, int y)
{
    // TODO : global to chunk and global to pos
    return 0;
}

void Map::load(pugi::xml_node& node)
{
}

void Map::save(pugi::xml_node& node)
{
    node.append_attribute("type") = "Map";
}

Map::Chunk* Map::getChunk(int cx, int cy)
{
    sf::Vector2i coords = sf::Vector2i(cx,cy);
    for (std::size_t i = 0; i < mChunks.size(); i++)
    {
        if (mChunks[i]->coords == coords)
        {
            return mChunks[i];
        }
    }
    return nullptr;
}
