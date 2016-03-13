#include "Forest.hpp"

Forest::Forest()
{
}

void Forest::init(int x, int y, std::size_t size)
{
    setCoords(x,y);
    for (std::size_t i = 0; i < size; i++)
    {
        sf::Vector2i c = sf::Vector2i(0,0);
        do
        {
            c.x = NMath::random(-1,1);
            c.y = NMath::random(-1,1);
        } while (0); // TODO : Fix it
        addSprite(c.x,c.y,sf::IntRect(512,0,256,256));
    }
}
