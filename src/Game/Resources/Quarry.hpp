#ifndef QUARRY_HPP
#define QUARRY_HPP

#include "Resources.hpp"

class Quarry : public Resource
{
    public:
        typedef std::shared_ptr<Quarry> Ptr;

    public:
        Quarry();
        Quarry(int x, int y);

        std::vector<std::pair<sf::Vector2i,sf::IntRect>> getTiles(int x, int y);
};

#endif // QUARRY_HPP
