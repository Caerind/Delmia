#include "Market.hpp"

Market::Market() : Building()
{
    mType = Buildings::Market;
}

Market::Market(sf::Vector2i const& coords) : Building(coords)
{
    mType = Buildings::Market;
    generate();
}

std::vector<sf::Vector2i> Market::getTilesBlueprint(sf::Vector2i const& coords)
{
    std::vector<sf::Vector2i> tiles;
    if (coords.y % 2 == 0)
    {
        tiles.push_back(coords);
        tiles.push_back(coords + sf::Vector2i(0,-1));
    }
    else
    {
        tiles.push_back(coords);
        tiles.push_back(coords + sf::Vector2i(1,-1));
    }
    return tiles;
}

std::vector<std::pair<sf::Vector2i,sf::IntRect>> Market::getTiles(sf::Vector2i const& coords)
{
    std::vector<sf::Vector2i> tilesBP = Market::getTilesBlueprint(coords);
    std::vector<std::pair<sf::Vector2i,sf::IntRect>> tiles;
    for (std::size_t i = 0; i < tilesBP.size(); i++)
    {
        tiles.emplace_back();
        tiles.back().first = tilesBP[i];
    }
    for (std::size_t i = 0; i < tiles.size(); i++)
    {
        tiles[i].second = sf::IntRect(1792,0,256,256);
    }
    return tiles;
}
