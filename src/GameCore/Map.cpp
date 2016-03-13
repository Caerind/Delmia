#include "Map.hpp"

Map::Map()
{
    mMapComponent.create("iso",{40,80},{256,128},NLayerComponent::Isometric);
    mRoadComponent.create("iso",{40,80},{256,128},NLayerComponent::Isometric);
    mMapComponent.setPosition(0,0,-2);
    mRoadComponent.setPosition(0,0,-1);
    sf::Vector2i coords;
    for (coords.x = 0; coords.x < 40; coords.x++)
    {
        for (coords.y = 0; coords.y < 80; coords.y++)
        {
            if (coords.y > 10 && coords.x > 10)
            {
                mMapComponent.setTileId(coords,3);
            }
            else
            {
                mMapComponent.setTileId(coords,1);
            }
            mRoadComponent.setTileId(coords,0);
        }
    }
}

void Map::setTileId(int x, int y, int id)
{
    mMapComponent.setTileId(sf::Vector2i(x,y),id);
}

void Map::setRoadId(int x, int y, int id)
{
    mRoadComponent.setTileId(sf::Vector2i(x,y),id);
}

int Map::getTileId(int x, int y)
{
    return mMapComponent.getTileId(sf::Vector2i(x,y));
}

int Map::getRoadId(int x, int y)
{
    return mRoadComponent.getTileId(sf::Vector2i(x,y));
}

void Map::load(pugi::xml_node& node)
{
    mMapComponent.load(node,"Map");
    mRoadComponent.load(node,"Road");
}

void Map::save(pugi::xml_node& node)
{
    node.append_attribute("type") = "Map";
    mMapComponent.save(node,"Map");
    mRoadComponent.save(node,"Road");
}
