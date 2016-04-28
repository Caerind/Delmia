#include "Market.hpp"

Market::Market() : Building()
{
}

Market::Market(int x, int y) : Building(x,y)
{
    generate();
}

std::vector<sf::Vector2i> Market::getTilesBlueprint(int x, int y)
{
    std::vector<sf::Vector2i> tiles;
    if (y % 2 == 0)
    {
        tiles.push_back(sf::Vector2i(x,y));
        tiles.push_back(sf::Vector2i(x,y-1));
    }
    else
    {
        tiles.push_back(sf::Vector2i(x,y));
        tiles.push_back(sf::Vector2i(x+1,y-1));
    }
    return tiles;
}

std::vector<std::pair<sf::Vector2i,sf::IntRect>> Market::getTiles(int x, int y)
{
    std::vector<sf::Vector2i> tilesBP = Market::getTilesBlueprint(x,y);
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
