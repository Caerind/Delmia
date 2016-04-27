#include "Forest.hpp"

Forest::Forest() : Resource(0,0)
{
    generate(0,0);
    addResource(Resources::Wood,1000);
}

Forest::Forest(int x, int y) : Resource(x,y)
{
    generate(x,y);
    addResource(Resources::Wood,1000);
}

std::vector<std::pair<sf::Vector2i,sf::IntRect>> Forest::getTiles(int x, int y)
{
    std::vector<sf::Vector2i> tilesBP = Building::getTilesBlueprint(x,y);
    std::vector<std::pair<sf::Vector2i,sf::IntRect>> tiles;
    for (std::size_t i = 0; i < tilesBP.size(); i++)
    {
        tiles.emplace_back();
        tiles.back().first = tilesBP[i];
    }
    tiles[0].second = sf::IntRect(512,0,256,256);
    return tiles;
}
