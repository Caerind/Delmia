#ifndef BARRACK_HPP
#define BARRACK_HPP

#include "Building.hpp"

class Barrack : public Building
{
    public:
        typedef std::shared_ptr<Barrack> Ptr;

    public:
        Barrack();
        Barrack(int x, int y);

        static std::vector<sf::Vector2i> getTilesBlueprint(int x, int y);
        std::vector<std::pair<sf::Vector2i,sf::IntRect>> getTiles(int x, int y);
};

#endif // BARRACK_HPP
