#ifndef FOREST_HPP
#define FOREST_HPP

#include "Resources.hpp"

class Forest : public Resource
{
    public:
        typedef std::shared_ptr<Forest> Ptr;

    public:
        Forest();
        Forest(int x, int y);

        std::vector<std::pair<sf::Vector2i,sf::IntRect>> getTiles(int x, int y);
};

#endif // FOREST_HPP
