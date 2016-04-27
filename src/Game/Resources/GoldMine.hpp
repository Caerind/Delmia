#ifndef GOLDMINE_HPP
#define GOLDMINE_HPP

#include "Resources.hpp"

class GoldMine : public Resource
{
    public:
        typedef std::shared_ptr<GoldMine> Ptr;

    public:
        GoldMine();
        GoldMine(int x, int y);

        std::vector<std::pair<sf::Vector2i,sf::IntRect>> getTiles(int x, int y);
};

#endif // GOLDMINE_HPP
