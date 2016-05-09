#include "Hall.hpp"

Hall::Hall() : Building()
{
    mType = Buildings::Hall;
}

Hall::Hall(sf::Vector2i const& coords) : Building(coords)
{
    mType = Buildings::Hall;
    generate();
}

std::vector<sf::Vector2i> Hall::getTilesBlueprint(sf::Vector2i const& coords)
{
    std::vector<sf::Vector2i> tiles;
    if (coords.y % 2 == 0)
    {
        tiles.push_back(coords);
        tiles.push_back(coords + sf::Vector2i(0,-1));
        tiles.push_back(coords + sf::Vector2i(1,-2));
    }
    else
    {
        tiles.push_back(coords);
        tiles.push_back(coords + sf::Vector2i(1,-1));
        tiles.push_back(coords + sf::Vector2i(1,-2));
    }
    return tiles;
}

std::vector<std::pair<sf::Vector2i,sf::IntRect>> Hall::getTiles(sf::Vector2i const& coords)
{
    std::vector<sf::Vector2i> tilesBP = Hall::getTilesBlueprint(coords);
    std::vector<std::pair<sf::Vector2i,sf::IntRect>> tiles;
    for (std::size_t i = 0; i < tilesBP.size(); i++)
    {
        tiles.emplace_back();
        tiles.back().first = tilesBP[i];
    }
    for (std::size_t i = 0; i < tiles.size(); i++)
    {
        tiles[i].second = sf::IntRect(0,0,256,256);
    }
    return tiles;
}
