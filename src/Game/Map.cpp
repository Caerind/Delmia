#include "Map.hpp"

Map::Map()
{
    mChunks.emplace_back();
    mChunks.back() = new Chunk();
    mChunks.back()->map.create("iso",getChunkSize(),getTileSize(),NLayerComponent::Isometric);
    mChunks.back()->road.create("iso",getChunkSize(),getTileSize(),NLayerComponent::Isometric);
    mChunks.back()->map.setPosition(0,0,-2);
    mChunks.back()->road.setPosition(0,0,-1);
    mChunks.back()->coords = sf::Vector2i(0,0);

    sf::Vector2i coords;
    for (coords.x = 0; coords.x < getChunkSize().x; coords.x++)
    {
        for (coords.y = 0; coords.y < getChunkSize().y; coords.y++)
        {
            if (coords.y > 10 && coords.x > 10)
            {
                mChunks.back()->map.setTileId(coords,3);
            }
            else
            {
                mChunks.back()->map.setTileId(coords,1);
            }
            mChunks.back()->road.setTileId(coords,0);
        }
    }
}

sf::Vector2i Map::getChunkSize() const
{
    return sf::Vector2i(16,64);
}

sf::Vector2i Map::getTileSize() const
{
    return sf::Vector2i(256,128);
}

void Map::setTileId(int x, int y, int id)
{
}

void Map::setRoadId(int x, int y, int id)
{
}

int Map::getTileId(int x, int y)
{
}

int Map::getRoadId(int x, int y)
{
}

void Map::load(pugi::xml_node& node)
{
}

void Map::save(pugi::xml_node& node)
{
    node.append_attribute("type") = "Map";
}
